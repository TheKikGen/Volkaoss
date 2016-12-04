// VOLKASSILATOR PROJECT
//
// Remap and control events for a better use of the Korg Kaossilator pro and the Volca Sample.
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// Franck Touanen - Nov 2016.

#include "build_number_defines.h"
#include <MIDI.h>

// Custom MDI setting

struct customMIDISettings : public midi::DefaultSettings {
    static const bool     UseRunningStatus = true;
    static const bool     HandleNullVelocityNoteOnAsNoteOff = true; // get NoteOff events when receiving null-velocity NoteOn messages.
    static const bool     Use1ByteParsing = true;
    static const long     BaudRate = 31250;
    static const unsigned SysExMaxSize = 128;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, customMIDISettings);

// GENERIC DEFINES =============================================================================

#define LED 13
#define ON  1
#define OFF 0

// MIDI CC AND OTHERS GLOBALS DEFINES ==========================================================
#define CC_MODULATION_WHEEL 1
#define CC_CHANNEL_VOLUME   7
#define CC_PAN              10
#define CC_REVERDB_DEPTH    91
#define CC_RESONNANCE       71
#define CC_RELEASE_TIME     72
#define CC_ATTACK_TIME      73
#define CC_CUTOFF           74
#define CC_DECAY            75
#define CC_LFO_RATE         76
#define CC_LFO_AMOUNT       77
#define CC_LFO_DELAY        78
#define CC_SUSTAIN          79

// VOLKAOSS INTERNAL DEFINES ===================================================================
#define VKINTERNAL_VERSION 1.0
#define VKINTERNAL_MIDI_IN 16

// GLOBALS =====================================================================================
byte          clockCounter = 0;
bool          playingStatus = false;
int           noteOnCounter = 0;            // Global NoteOn Counter
unsigned long lastMidiMessageTimestamp =0;  // To handle the LED ON / Off

// DEVICE SPECIFIC DEFINES ==================================================================
#include "Volca_Sample_Defines.h"
#include "Kaossilator_Defines.h"

// FUNCS =======================================================================================

// REAL TIME EVENTS ----------------------------------------------------------------------------
void OnMidiClock() {
  clockCounter++;
  if (clockCounter == 1) {
      digitalWrite(LED, HIGH); // set the LED on
      clockTrigEvents();
  }
  // do something every 16th note

  else if (clockCounter == 6 ) digitalWrite(LED, LOW); // set the LED off

// do something every 8th note
//  else if (clockCounter == 12 ) {  }  ;

  // do something every quarter note
  else if (clockCounter >= 24 ) clockCounter = 0 ;
}

void clockTrigEvents() {

  KaossTrigEvents();
  
}

void OnStart() {
  playingStatus = true;
  clockCounter = 0;      // reset the counter, start from the top
}

void OnContinue() {
  playingStatus = true;
}

void OnStop() {
  playingStatus = false;
  KaossProcessStop();
}


// Midi Handlers -------------------------------------------------------------------------------

void OnNoteOff(byte channel, byte note, byte velocity) {
  noteOnCounter --;
  if (channel == kaossMidiIn) KaossProcessNoteOff(channel, note, velocity);
  //if ( noteOnCounter<1 ) digitalWrite(LED, LOW);
}

// We assume that velocity 0 is never received here because of MIDI custom settings.
// So, we don't have to test the velocity > 0 every time
void OnNoteOn(byte channel, byte note, byte velocity) {
  noteOnCounter++;
  //digitalWrite(LED, HIGH);

  if (channel == kaossMidiIn) KaossProcessNoteOn(channel, note, velocity);
  else if (channel == VSAMPLE_MIDI_IN or bitRead(vSampleccMidiInState,channel) ) VSampleProcessNoteOn(channel, note, velocity);
}

void OnProgramChange(byte channel, byte number) {

  if (channel == kaossMidiIn) KaossProcessProgramChange(channel, number);
  else MIDI.sendProgramChange(number, channel);
}

void OnControlChange(byte channel, byte control, byte value) {
  if (channel == kaossMidiIn) KaossProcessControlChange(channel, control, value);
  else if (channel == VSAMPLE_MIDI_IN || bitRead(vSampleccMidiInState,channel) ) VSampleProcessControlChange(channel, control, value) ;
}

void OnPitchBend(byte channel, int pitch) {
  if (channel == kaossMidiIn) KaossProcessPitchBend(channel, pitch);
}

// Utilities -----------------------------------------------------------------------------------

void MIDIhardreset() {
Serial.write(0xFF);
}

void MIDIsoftreset() {
Serial.write(0xF0);
Serial.write(0x7F);
Serial.write(0x7F); //device id 7F = 127 all channels
Serial.write(0x02);
Serial.write(0x7F); // command format 7F = all devices
Serial.write(0x0A); // action 0x0A= reset
Serial.write(0xF7);
}

void BlinkLed(byte num)         // Basic LED blink function
{
    for (byte i=0;i<num;i++) {
        digitalWrite(LED,HIGH);
        delay(50);
        digitalWrite(LED,LOW);
        delay(50);
    }
}

//MIDI All Notes Off (Can be handy during development and as a general power up command to send)
void MIDIAllNotesOff() {
  //Serial.write(0xB0 + 11);
  Serial.write(0x7F);
  Serial.write(0x7B);
  Serial.write(0x00);
}

// Main ----------------------------------------------------------------------------------------
void setup() {


  // Show the build number
  Serial.begin(115200);
  Serial.println("Volkaoss MIDI controller.  Build number : ");
  Serial.println(TimestampedVersion);
  Serial.println();
  Serial.println("==========================================");
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  
  
  delay(3000);
  
  pinMode(LED, OUTPUT);

  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //MIDI.turnThruOff(); // ALWAYS AFTER BEGIN ELSE IT DOESN'T WORK !!
  MIDI.turnThruOn(3); // Off but system messages
/*
    Off                   = 0,  ///< Thru disabled (nothing passes through).
    Full                  = 1,  ///< Fully enabled Thru (every incoming message is sent back).
    SameChannel           = 2,  ///< Only the messages on the Input Channel will be sent back.
    DifferentChannel      = 3,  ///< All the messages but the ones on the Input Channel will be sent back.
};*/

  MIDI.setHandleNoteOn(OnNoteOn);
  MIDI.setHandleNoteOff(OnNoteOff);
  MIDI.setHandleControlChange(OnControlChange);
  MIDI.setHandleProgramChange(OnProgramChange) ;
  MIDI.setHandleClock(OnMidiClock);
  MIDI.setHandleStart(OnStart);
  MIDI.setHandleContinue(OnContinue);
  MIDI.setHandleStop(OnStop);
  MIDI.setHandlePitchBend(OnPitchBend);

  MIDIAllNotesOff();

  VSampleInitialize();
  KaossInitialize();
}

void loop() {
  if ( MIDI.read() ) {
    lastMidiMessageTimestamp = millis();
    digitalWrite(LED, HIGH);
  }
  else if ( millis() - lastMidiMessageTimestamp > 1000) {
    digitalWrite(LED, LOW);
  } 
   
}
