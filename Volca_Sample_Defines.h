// VOLKASSILATOR PROJECT
//
// Specific Vocla Sample defines /globals and functions
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// Franck Touanen - Nov 2016.

// VOLCA SAMPLE DEFINES ========================================================================

#define VSAMPLE_MIDI_IN 10         // Capture MIDI in channel for the Volva Sample device
                                   // Note : MIDI channels out are always 1-10 for the Volca Sample.
#define VSAMPLE_CC_MIDI_IN_FROM 5  // For detailed CC setting on parts, we reserve channel 5 to 9
#define VSAMPLE_CC_MIDI_IN_TO   9  // Channels are activable independantly with commands at C0 on the last part used

#define VSAMPLE_ROOT_GM_KEY 36         // C2. Key used to trig a sample and mapping to channels in drums GM pseudo mode
#define VSAMPLE_ROOT_CHROMATIC_KEY 48  // C3. Key used to trig a sample chromatically

// Volca sample CC messages

#define VSAMPLE_CC_LEVEL               7
#define VSAMPLE_CC_PAN                10
#define VSAMPLE_CC_START_POINT        40
#define VSAMPLE_CC_LENGTH             41
#define VSAMPLE_CC_HI_CUT             42
#define VSAMPLE_CC_SPEED              43
#define VSAMPLE_CC_PITCH_EG_INT       44
#define VSAMPLE_CC_PITCH_EG_ATTACK    45
#define VSAMPLE_CC_PITCH_EG_DECAY     46
#define VSAMPLE_CC_AMP_ATTACK         47
#define VSAMPLE_CC_AMP_DECAY          48

// Commands are at C0

#define VSAMPLE_CMD_TOGGLE_CC_MIDI_IN     12 // Midi IN on/off for the selected part (channel)  
#define VSAMPLE_CMD_RESET_ALL_CC          14 // Reset all CC value of the current part

/*16  Gen Purpose Controller 1
17  Gen Purpose Controller 2
18  Gen Purpose Controller 3
19  Gen Purpose Controller 4*/

// GLOBALS =====================================================================================

byte vSampleLastNoteOnChannel = 0 ;
int  vSampleccMidiInState = 0 ;   // if the (channel) bit is 1 , then MIDI IN is ON

// PITCH VALUE FOR VOLCA SAMPLE WHEN USING THE SPEED CC
const byte VSAMPLE_NOTE_TO_CC_SPEED[] = {
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, // C3
32, 34, 37, 40, 43, 45, 48, 51, 53, 56, 59, 61, // C4
64, 67, 69, 72, 75, 78, 80, 83, 85, 88, 91, 93, // C5
96, 97, 99, 100, 102, 103, 104, 106, 107, 108,  // C6
109, 111, 112
};

// Volca Sample funcs --------------------------------------------------------------------------

void VSampleBlinkedChannels(byte num) {
    for (byte i=0;i<num;i++)
    {
      for (byte channel=4; channel<= 7 ; channel++ ) {
          MIDI.sendControlChange(VSAMPLE_CC_LEVEL, 0, channel );
          MIDI.sendNoteOn(VSAMPLE_ROOT_GM_KEY,127, channel);   }
      delay(200);    }
}

void VSampleResetAllCC(byte channel) {
    if (channel >=1 || channel <=10 ) {
        MIDI.sendControlChange(VSAMPLE_CC_START_POINT, 0, channel );
        MIDI.sendControlChange(VSAMPLE_CC_LENGTH, 127, channel );
        MIDI.sendControlChange(VSAMPLE_CC_HI_CUT, 127, channel );
        MIDI.sendControlChange(VSAMPLE_CC_SPEED, 64, channel );
        MIDI.sendControlChange(VSAMPLE_CC_PITCH_EG_INT, 64, channel );
        MIDI.sendControlChange(VSAMPLE_CC_PITCH_EG_ATTACK, 0, channel );
        MIDI.sendControlChange(VSAMPLE_CC_PITCH_EG_DECAY, 127, channel );
        MIDI.sendControlChange(VSAMPLE_CC_LEVEL,100, channel );
        MIDI.sendControlChange(VSAMPLE_CC_PAN,64, channel );
        MIDI.sendControlChange(VSAMPLE_CC_AMP_ATTACK, 0, channel );
        MIDI.sendControlChange(VSAMPLE_CC_AMP_DECAY, 127, channel );
    }
}

void VSampleToggleCCMidiIn(byte channel) {
    // Midi channels are activables only in the specified range in the defines
    if ( channel >= VSAMPLE_CC_MIDI_IN_FROM && channel <= VSAMPLE_CC_MIDI_IN_TO ) bitWrite(vSampleccMidiInState,channel,!bitRead(vSampleccMidiInState,channel));
}

void VSampleProcessNoteOn(byte channel, byte note, byte velocity) {
  static int thisVar;

  // CMD Only on the channel 10
  if (channel == VSAMPLE_MIDI_IN ) {

    // Reset all CC on the current part
    if ( note == VSAMPLE_CMD_RESET_ALL_CC && vSampleLastNoteOnChannel >0) VSampleResetAllCC(vSampleLastNoteOnChannel);

    // Midi IN  channel ON / OFF
    else if ( note == VSAMPLE_CMD_TOGGLE_CC_MIDI_IN && vSampleLastNoteOnChannel >0 )  VSampleToggleCCMidiIn(vSampleLastNoteOnChannel);
      
    // Play note in GM Mode
    else if ( note >= VSAMPLE_ROOT_GM_KEY && note < (VSAMPLE_ROOT_GM_KEY + 10)  ) {
        thisVar = constrain( note - VSAMPLE_ROOT_GM_KEY +1,0,127);
        MIDI.sendControlChange(CC_CHANNEL_VOLUME, velocity, thisVar);  // Velocity simulation
        MIDI.sendNoteOn(VSAMPLE_ROOT_GM_KEY,127, thisVar);
        vSampleLastNoteOnChannel = thisVar ;
    } 
    // Play note chromatically in GM mode  - Channel 10
    else if ( note >= VSAMPLE_ROOT_CHROMATIC_KEY && note <= 127 && vSampleLastNoteOnChannel ) {
        MIDI.sendControlChange(CC_CHANNEL_VOLUME, velocity, vSampleLastNoteOnChannel);  // Velocity simulation
        MIDI.sendControlChange(VSAMPLE_CC_SPEED, VSAMPLE_NOTE_TO_CC_SPEED[note], vSampleLastNoteOnChannel );
        MIDI.sendNoteOn(VSAMPLE_ROOT_CHROMATIC_KEY,127, vSampleLastNoteOnChannel);
    }
  }
  // Play notes chromatically on an activated channel (for MIDI recording purpose)
  else if ( bitRead(vSampleccMidiInState,channel) ) {
        // Other channel than 10, in the (FROM, TO) range play on the full range of the keyboard only if they are active
        MIDI.sendControlChange(CC_CHANNEL_VOLUME, velocity, channel);  // Velocity simulation
        MIDI.sendControlChange(VSAMPLE_CC_SPEED, VSAMPLE_NOTE_TO_CC_SPEED[note], channel );
        MIDI.sendNoteOn(VSAMPLE_ROOT_CHROMATIC_KEY,127, channel);    
  }
}

void VSampleProcessControlChange(byte channel, byte control, byte value) {
    byte xchannel;
    
    xchannel = channel;
    if ( channel == VSAMPLE_MIDI_IN or bitRead(vSampleccMidiInState,channel) ) {      
        if ( channel == VSAMPLE_MIDI_IN) xchannel = vSampleLastNoteOnChannel; 
        switch (control) {
            case CC_CHANNEL_VOLUME:  MIDI.sendControlChange(VSAMPLE_CC_LEVEL,           value, xchannel); break;
            case CC_PAN:             MIDI.sendControlChange(VSAMPLE_CC_PAN,             value, xchannel); break;
            case CC_ATTACK_TIME:     MIDI.sendControlChange(VSAMPLE_CC_START_POINT,     value, xchannel); break;
            case CC_RELEASE_TIME:    MIDI.sendControlChange(VSAMPLE_CC_LENGTH,          value, xchannel); break;
            case CC_CUTOFF:          MIDI.sendControlChange(VSAMPLE_CC_HI_CUT,          value, xchannel); break;
            case CC_LFO_RATE:        MIDI.sendControlChange(VSAMPLE_CC_PITCH_EG_INT,    value, xchannel); break;
            case CC_LFO_AMOUNT:      MIDI.sendControlChange(VSAMPLE_CC_PITCH_EG_ATTACK, value, xchannel); break;
            case CC_LFO_DELAY:       MIDI.sendControlChange(VSAMPLE_CC_PITCH_EG_DECAY,  value, xchannel); break;
            case CC_SUSTAIN:         MIDI.sendControlChange(VSAMPLE_CC_AMP_ATTACK,      value, xchannel); break;                                                            
            case CC_DECAY:           MIDI.sendControlChange(VSAMPLE_CC_AMP_DECAY,       value, xchannel); break;
            //default:                 MIDI.sendControlChange(control,                    value, xchannel); break;
        }
    }
}

void VSampleInitialize() {

  for (byte channel=1; channel<=10 ; channel++) {
    MIDI.sendControlChange(VSAMPLE_CC_LEVEL, 0, channel );
    MIDI.sendNoteOn(VSAMPLE_ROOT_GM_KEY,127, channel);  // Light the channel on the Volcal Sample
    VSampleResetAllCC(channel);
    delay(200);
  }

  VSampleBlinkedChannels(4);

}


