// THE VOLKASSILATOR PROJECT
//
// Specific Kaossilator pro defines
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// Franck Touanen - Nov 2016.

// ====================================================================
// KAOSSILATOR PRO  DEFINES 
// ====================================================================
#define KAOSS_MIDI_IN             1  // Default MIDI in channel to capture events to Kaosspad
#define KAOSS_MIDI_OUT            11  // Remember that when  using Volcasample, channel 1-10 are used
#define KAOSS_CC_PAD              92  // pad on/off control change # (check the manual for more information)
#define KAOSS_CC_X                12  // pad on/off control change # (check the manual for more information)
#define KAOSS_CC_Y                13  // pad on/off control change # (check the manual for more information)
#define KAOSS_CC_GATE_ARP_SW      90  // GATE ARP on/off Control change
#define KAOSS_CC_GATE_ARP_SPEED   91  // GATE ARP speed Control change
#define KAOSS_CC_GATE_ARP_TIME    93  // GATE ARP time  Control change
#define KAOSS_NOTEON_BANK_A       36  // LOOP RECORDER BANK A button Note on/off C2
#define KAOSS_NOTEON_BANK_B       37  // LOOP RECORDER BANK B button Note on/off C#2
#define KAOSS_NOTEON_BANK_C       38  // LOOP RECORDER BANK C button Note on/off D2
#define KAOSS_NOTEON_BANK_D       39  // LOOP RECORDER BANK D button Note on/off D#2
#define KAOSS_CC_VOLUME           94  // PROGRAM VOLUME knob Control change

// Commands are at C0
// U ' U ' U U ' U ' U ' U
// 12  14  16    19  21  23   

#define KAOSS_CMD_MODE_KEY            12 // Command mode root Key - C0

#define KAOSS_CMD_BANK_A              KAOSS_CMD_MODE_KEY +2 
#define KAOSS_CMD_BANK_B              KAOSS_CMD_MODE_KEY +4 
#define KAOSS_CMD_BANK_C              KAOSS_CMD_MODE_KEY +5
#define KAOSS_CMD_BANK_D              KAOSS_CMD_MODE_KEY + 7
#define KAOSS_CMD_ALLBANKS_OFF        KAOSS_CMD_MODE_KEY + 1
#define KAOSS_CMD_ALLBANKS_ON         KAOSS_CMD_MODE_KEY + 3
#define KAOSS_CMD_GATE_ARP_SPEED_DEC  KAOSS_CMD_MODE_KEY + 8 
#define KAOSS_CMD_GATE_ARP            KAOSS_CMD_MODE_KEY + 9
#define KAOSS_CMD_GATE_ARP_SPEED_INC  KAOSS_CMD_MODE_KEY +10
#define KAOSS_CMD_SET_MIDI_CHANNEL    KAOSS_CMD_MODE_KEY +11 

// Bank on/off status Bit mask
#define KAOSS_BANK_STATE_A        0
#define KAOSS_BANK_STATE_B        1
#define KAOSS_BANK_STATE_C        2
#define KAOSS_BANK_STATE_D        3
#define KAOSS_GATE_ARP_STATE      4

// ====================================================================
// GLOBALS
// ====================================================================

// KAOSS BANK TRIGGERS QUANTIZED ON / OFF globals
// KAOSS COMMANDS BASED ON events synchronized with the MIDI clock
byte kaossMidiIn=KAOSS_MIDI_IN;
byte kaossMidiInTemp=0;
bool kaossClockTriggerBankA=false;
bool kaossClockTriggerBankB=false;
bool kaossClockTriggerBankC=false;
bool kaossClockTriggerBankD=false;
bool kaossClockTriggerGateArp=false;
bool kaossClockTriggerAllBanksOn=false;
int  kaossGateArpSpeed = 0;
byte kaossGateArpSpeedCCValues[] = {0,2,14,22,40,54,64,84,94,118,127};
byte kaossButtonsState=0;

byte kaossXPad  = 0; 
byte kaossYPad  = 64; // Start at the middle because at 0, sometimes no sound, depending on patches
byte kaossXPadPress = 0;
byte kaossYPadPress = 0;

int  kaossNoteOn = 0 ;             // Kaoss pad note on dedicated note on counter
int  kaossNoteTranspose = 36 ;     // Offset from C-1
int  kaossPitchBendMSB =0;

bool kaossCommandModeKeyPressed=false;

// MAP MIDI NOTES TO KAOSS CC
// CONFIGURATION : 4 OCTAVES from C-1  to C2 + C3 -  48 + 1 NOTES - SCALE CHROMATIC KEY C
// C0 is reserved for commands. Do not transpose at +12.

const byte KAOSS_NOTE_TO_CC[] = {
//  W    B    W   B     W    W    B    W   B    W    B    W
    1,   4,   7,   9,  12,  15,  17,  20,  22,  25,  28,  30,
    33, 35,  38,  41,  43,  46,  49,  51,  54,  56,  59,  62,
    64, 67,  69,  72,  75,  77,  80,  82,  86,  88,  90,  93,
    96, 98, 101, 103, 106, 109, 111, 114, 116, 120, 122, 124
  ,127
};

// ====================================================================
// Kaoss Funcs
// ====================================================================

void KaossToggleBank(byte ccBank, byte bankState) {
    bitWrite(kaossButtonsState,bankState,!bitRead(kaossButtonsState,bankState));
    MIDI.sendNoteOn(ccBank,127, KAOSS_MIDI_OUT);
}

void KaossSetAllBanks(bool banksOn) {
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_A) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_A,127, KAOSS_MIDI_OUT);
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_B) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_B,127, KAOSS_MIDI_OUT);
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_C) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_C,127, KAOSS_MIDI_OUT);
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_D) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_D,127, KAOSS_MIDI_OUT);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_A,banksOn);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_B,banksOn);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_C,banksOn);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_D,banksOn);
}

void KaossProcessPitchBend(byte channel, int pitch) {
  kaossPitchBendMSB = MIDI.getData2() -64 ;  
  MIDI.sendControlChange(KAOSS_CC_X, constrain(kaossXPad + kaossPitchBendMSB ,0,127), KAOSS_MIDI_OUT);    // Use only MSB here (data 2)
}

void KaossSetGateArp(bool state) {
    bitWrite(kaossButtonsState,KAOSS_GATE_ARP_STATE,state);
    MIDI.sendControlChange(KAOSS_CC_GATE_ARP_SW, state ? 127 : 0, KAOSS_MIDI_OUT);
}

void KaossGateArpSetSpeed(bool speedIncrease) {
    if (speedIncrease) kaossGateArpSpeed++ ; else kaossGateArpSpeed --;
    if ( kaossGateArpSpeed > 10) kaossGateArpSpeed = 0;
    else if ( kaossGateArpSpeed < 0) kaossGateArpSpeed = 10;
    MIDI.sendControlChange(KAOSS_CC_GATE_ARP_SPEED, kaossGateArpSpeedCCValues[kaossGateArpSpeed], KAOSS_MIDI_OUT);
}

void KaossInitialize() {
    MIDI.sendControlChange(KAOSS_CC_VOLUME, 64,KAOSS_MIDI_OUT); 
    KaossSetAllBanks(ON);
    delay(200);
    KaossSetAllBanks(OFF);
    KaossSetGateArp(OFF);
    delay(200);
    KaossToggleBank(KAOSS_NOTEON_BANK_A,KAOSS_BANK_STATE_A);
    delay(200);
    KaossToggleBank(KAOSS_NOTEON_BANK_A,KAOSS_BANK_STATE_A);
    KaossToggleBank(KAOSS_NOTEON_BANK_B,KAOSS_BANK_STATE_B);
    delay(200);
    KaossToggleBank(KAOSS_NOTEON_BANK_B,KAOSS_BANK_STATE_B);
    KaossToggleBank(KAOSS_NOTEON_BANK_C,KAOSS_BANK_STATE_C);
    delay(200);
    KaossToggleBank(KAOSS_NOTEON_BANK_C,KAOSS_BANK_STATE_C);
    KaossToggleBank(KAOSS_NOTEON_BANK_D,KAOSS_BANK_STATE_D);
    delay(200);
    KaossSetAllBanks(OFF);
    MIDI.sendControlChange(KAOSS_CC_GATE_ARP_SPEED, kaossGateArpSpeedCCValues[0], KAOSS_MIDI_OUT);
    MIDI.sendControlChange(KAOSS_CC_VOLUME, 127,KAOSS_MIDI_OUT); 
}

void KaossProcessStop() {
  // if somes notes were on, we have to "note off" them. Hopefully, Kaoss is a mono synth
  // So we have only to PAD OFF and reset counters. Not the same story in POLY mode
  if ( kaossNoteOn ) {               
      MIDI.sendControlChange(KAOSS_CC_PAD, 0,KAOSS_MIDI_OUT); // PAD OFF
      kaossNoteOn = 0;      
      kaossYPadPress = kaossXPadPress = kaossXPad = 0 ;       // We keep only Y
  }
  KaossSetAllBanks(OFF); 
}

// Events to trig when clock rises
void KaossTrigEvents() {
  
  if (kaossClockTriggerBankA) {
    KaossToggleBank(KAOSS_NOTEON_BANK_A,KAOSS_BANK_STATE_A);
    kaossClockTriggerBankA=false;
  }

  if (kaossClockTriggerBankB) {
    KaossToggleBank(KAOSS_NOTEON_BANK_B,KAOSS_BANK_STATE_B);
    kaossClockTriggerBankB=false;
  }

  if (kaossClockTriggerBankC) {
    KaossToggleBank(KAOSS_NOTEON_BANK_C,KAOSS_BANK_STATE_C);
    kaossClockTriggerBankC=false;
  }

  if (kaossClockTriggerBankD) {
    KaossToggleBank(KAOSS_NOTEON_BANK_D,KAOSS_BANK_STATE_D);
    kaossClockTriggerBankD=false;
  }

  if (kaossClockTriggerAllBanksOn) {
    KaossSetAllBanks(ON);
    kaossClockTriggerAllBanksOn=false;
  }

  if (kaossClockTriggerGateArp) {
    KaossSetGateArp(ON);
    kaossClockTriggerGateArp=false;
  }

}

void KaossProcessNoteOn(byte channel, byte note, byte velocity) {
  static int thisVar;
    
  if (note == KAOSS_CMD_MODE_KEY) kaossCommandModeKeyPressed=true;
  else if ( kaossCommandModeKeyPressed ) {        // pressed and not released (no note off)
      // KAOSS Commands
      // NB : Take care that commands at C0 stay below notes to avoid noteon counter confusion
        switch (note) {      
          case KAOSS_CMD_BANK_A:
            // At the next beat if playing
            if (playingStatus && bitRead(kaossButtonsState,KAOSS_BANK_STATE_A)==0) kaossClockTriggerBankA = true;
            else KaossToggleBank(KAOSS_NOTEON_BANK_A,KAOSS_BANK_STATE_A) ;
            break;
          case KAOSS_CMD_BANK_B:
            if (playingStatus && bitRead(kaossButtonsState,KAOSS_BANK_STATE_B)==0) kaossClockTriggerBankB = true;
            else KaossToggleBank(KAOSS_NOTEON_BANK_B,KAOSS_BANK_STATE_B) ;
            break;
          case KAOSS_CMD_BANK_C:
            if (playingStatus && bitRead(kaossButtonsState,KAOSS_BANK_STATE_C)==0) kaossClockTriggerBankC = true;
            else KaossToggleBank(KAOSS_NOTEON_BANK_C,KAOSS_BANK_STATE_C) ;
            break;
          case KAOSS_CMD_BANK_D:
            if (playingStatus && bitRead(kaossButtonsState,KAOSS_BANK_STATE_D)==0) kaossClockTriggerBankD = true;
            else KaossToggleBank(KAOSS_NOTEON_BANK_D,KAOSS_BANK_STATE_D) ;
            break;
          case KAOSS_CMD_ALLBANKS_ON:
            if (playingStatus) kaossClockTriggerAllBanksOn = true;
            else KaossSetAllBanks(ON);
            break;
          case KAOSS_CMD_ALLBANKS_OFF:
            KaossSetAllBanks(OFF);
            break;
          case KAOSS_CMD_GATE_ARP:
            if ( bitRead(kaossButtonsState,KAOSS_GATE_ARP_STATE)==1 ) KaossSetGateArp(OFF);
            else kaossClockTriggerGateArp = true; ;
            break;
          case KAOSS_CMD_GATE_ARP_SPEED_INC:
            KaossGateArpSetSpeed(true);
            break;
          case KAOSS_CMD_GATE_ARP_SPEED_DEC:
            KaossGateArpSetSpeed(false);
            break;
          case KAOSS_CMD_SET_MIDI_CHANNEL:
            if ( ++kaossMidiInTemp > 16 ) kaossMidiInTemp = 1; 
            break;
      }
  }
  
  thisVar = note - kaossNoteTranspose;
  if ( thisVar >=0 && thisVar <= (sizeof(KAOSS_NOTE_TO_CC)-1) ){
      kaossXPad = KAOSS_NOTE_TO_CC[thisVar];          
      MIDI.sendControlChange(KAOSS_CC_VOLUME, velocity,KAOSS_MIDI_OUT); // Simulate Velocity;
      // X, Y 
      MIDI.sendControlChange(KAOSS_CC_X, constrain(kaossXPad + kaossPitchBendMSB ,0,127), KAOSS_MIDI_OUT);
      MIDI.sendControlChange(KAOSS_CC_Y, kaossYPad, KAOSS_MIDI_OUT);
      // Press Pad if only one note holded
      if ( ++kaossNoteOn == 1 ) {               
        MIDI.sendControlChange(KAOSS_CC_PAD, 127,KAOSS_MIDI_OUT); // PAD PRESS;               
        // Save coordinates when the pad was pressed the first time
        kaossXPadPress = kaossXPad; 
        kaossYPadPress = kaossYPad;
      }
  }
}

void KaossProcessNoteOff(byte channel, byte note, byte velocity) {
  static int thisVar;
  // NB : Take care that commands stay below notes to avoid noteon counter confusion
  if (note == KAOSS_CMD_MODE_KEY) {
      kaossCommandModeKeyPressed=false;
      if ( kaossMidiInTemp > 0 ) {           
          kaossMidiIn = kaossMidiInTemp ; // Channel change is applied only when Command Key is released
          kaossMidiInTemp = 0;
      }
  } 
  else {
      thisVar = note - kaossNoteTranspose;      
      if ( thisVar >=0 && thisVar <= (sizeof(KAOSS_NOTE_TO_CC)-1) ) {
         if ( --kaossNoteOn <1 ) {
            // Release PAD
            MIDI.sendControlChange(KAOSS_CC_PAD, 0, KAOSS_MIDI_OUT);
            kaossNoteOn = 0; // Secure NoteOn
            kaossYPadPress = kaossXPadPress = kaossXPad = 0 ;   // We keep only Y
         }
         // Restore initial X,Y values when pad pressed the first time (MONO behaviour)
         else {
            // Initial note released ? Change X,Y to recall
            if ( KAOSS_NOTE_TO_CC[thisVar] == kaossXPadPress) {
                kaossXPadPress = kaossXPad; 
                kaossYPadPress = kaossYPad;
            }
            // Only one note playing : restoring X,Y
            if ( kaossNoteOn == 1 ) {
              kaossXPad=kaossXPadPress ;
              kaossYPad=kaossYPadPress ;
              MIDI.sendControlChange(KAOSS_CC_X, constrain(kaossXPad + kaossPitchBendMSB ,0,127), KAOSS_MIDI_OUT);
              MIDI.sendControlChange(KAOSS_CC_Y, kaossYPad, KAOSS_MIDI_OUT);    
            }
         }
      }
  }
}

// Nothing to do but change the channel
void KaossProcessProgramChange(byte channel, byte number) {
  MIDI.sendProgramChange(number, KAOSS_MIDI_OUT);
}
  
void KaossProcessControlChange(byte channel, byte control, byte value) {
      if ( control == CC_MODULATION_WHEEL || control == CC_REVERDB_DEPTH || control == CC_CUTOFF ) {
          kaossYPad = value;
          MIDI.sendControlChange(KAOSS_CC_Y, kaossYPad, KAOSS_MIDI_OUT);
      } 
      else if (control == CC_CHANNEL_VOLUME ) {
          MIDI.sendControlChange(KAOSS_CC_VOLUME, value, KAOSS_MIDI_OUT);
      } 
      else if (control == CC_RESONNANCE  ) {
          kaossXPad = value;
          MIDI.sendControlChange(KAOSS_CC_X, value, KAOSS_MIDI_OUT);
      } 
      else MIDI.sendControlChange(control, value, KAOSS_MIDI_OUT);
}
