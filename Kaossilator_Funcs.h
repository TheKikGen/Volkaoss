// THE VOLKASSILATOR PROJECT
//
// Specific Kaossilator pro defines
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// Franck Touanen - Nov 2016.

// ====================================================================
// KAOSSILATOR PRO  FUNCS
// ====================================================================

void KaossToggleBank(byte ccBank, byte bankState) {
    bitWrite(kaossButtonsState,bankState,!bitRead(kaossButtonsState,bankState));
    MIDI.sendNoteOn(ccBank,127, kaossMidiOut);
}

void KaossSetAllBanks(bool banksOn) {
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_A) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_A,127, kaossMidiOut);
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_B) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_B,127, kaossMidiOut);
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_C) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_C,127, kaossMidiOut);
    if ( bitRead(kaossButtonsState,KAOSS_BANK_STATE_D) != banksOn ) MIDI.sendNoteOn(KAOSS_NOTEON_BANK_D,127, kaossMidiOut);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_A,banksOn);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_B,banksOn);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_C,banksOn);
    bitWrite(kaossButtonsState,KAOSS_BANK_STATE_D,banksOn);
}

void KaossProcessPitchBend(byte channel, int pitch) {
  kaossPitchBendMSB = MIDI.getData2() -64 ;
  MIDI.sendControlChange(KAOSS_CC_X, constrain(kaossXPad + kaossPitchBendMSB ,0,127), kaossMidiOut);    // Use only MSB here (data 2)
}

void KaossSetGateArp(bool state) {
    bitWrite(kaossButtonsState,KAOSS_GATE_ARP_STATE,state);
    MIDI.sendControlChange(KAOSS_CC_GATE_ARP_SW, state ? 127 : 0, kaossMidiOut);
}

void KaossGateArpSetSpeed(bool speedIncrease) {
    if (speedIncrease) kaossGateArpSpeed++ ; else kaossGateArpSpeed --;
    if ( kaossGateArpSpeed > 10) kaossGateArpSpeed = 0;
    else if ( kaossGateArpSpeed < 0) kaossGateArpSpeed = 10;
    MIDI.sendControlChange(KAOSS_CC_GATE_ARP_SPEED, kaossGateArpSpeedCCValues[kaossGateArpSpeed], kaossMidiOut);
}

void KaossInitialize() {
    MIDI.sendControlChange(KAOSS_CC_VOLUME, 64,kaossMidiOut);
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
    MIDI.sendControlChange(KAOSS_CC_GATE_ARP_SPEED, kaossGateArpSpeedCCValues[0], kaossMidiOut);
    MIDI.sendControlChange(KAOSS_CC_VOLUME, 127,kaossMidiOut);
}

void KaossProcessStop() {
  // if somes notes were on, we have to "note off" them. Hopefully, Kaoss is a mono synth
  // So we have only to PAD OFF and reset counters. Not the same story in POLY mode
  if ( kaossNoteOn ) {
      MIDI.sendControlChange(KAOSS_CC_PAD, 0,kaossMidiOut); // PAD OFF
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
          // Beyond 15, we cancel.
          case KAOSS_CMD_SET_MIDI_CHANNEL_IN: 
               kaossMidiInTemp++;
               break;
          case KAOSS_CMD_SET_MIDI_CHANNEL_OUT:
               kaossMidiOutTemp++;
               break;
      }
  }

  thisVar = note - kaossNoteTranspose;
  if ( thisVar >=0 && thisVar <= (sizeof(KAOSS_NOTE_TO_CC)-1) ){
      kaossXPad = KAOSS_NOTE_TO_CC[thisVar];
      MIDI.sendControlChange(KAOSS_CC_VOLUME, velocity,kaossMidiOut); // Simulate Velocity;
      // X, Y
      MIDI.sendControlChange(KAOSS_CC_X, constrain(kaossXPad + kaossPitchBendMSB ,0,127), kaossMidiOut);
      MIDI.sendControlChange(KAOSS_CC_Y, kaossYPad, kaossMidiOut);
      // Press Pad if only one note holded
      if ( ++kaossNoteOn == 1 ) {
        MIDI.sendControlChange(KAOSS_CC_PAD, 127,kaossMidiOut); // PAD PRESS;
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
      // Beyond 15 we consider that is equivalent to a cancelation
      if ( kaossMidiInTemp >= 1 ) {
          if ( kaossMidiInTemp <= 15 )kaossMidiIn = kaossMidiInTemp ; // Channel change is applied only when Command Key is released
          kaossMidiInTemp = 0;
      }
      if ( kaossMidiOutTemp >= 1 ) {
          if ( kaossMidiOutTemp <= 15 )kaossMidiOut = kaossMidiOutTemp ; // Channel change is applied only when Command Key is released
          kaossMidiOutTemp = 0;
      }
  }
  else {
      thisVar = note - kaossNoteTranspose;
      if ( thisVar >=0 && thisVar <= (sizeof(KAOSS_NOTE_TO_CC)-1) ) {
         if ( --kaossNoteOn <1 ) {
            // Release PAD
            MIDI.sendControlChange(KAOSS_CC_PAD, 0, kaossMidiOut);
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
              MIDI.sendControlChange(KAOSS_CC_X, constrain(kaossXPad + kaossPitchBendMSB ,0,127), kaossMidiOut);
              MIDI.sendControlChange(KAOSS_CC_Y, kaossYPad, kaossMidiOut);
            }
         }
      }
  }
}

// Nothing to do but change the channel
void KaossProcessProgramChange(byte channel, byte number) {
  MIDI.sendProgramChange(number, kaossMidiOut);
}

void KaossProcessControlChange(byte channel, byte control, byte value) {
      if ( control == CC_MODULATION_WHEEL || control == CC_REVERDB_DEPTH || control == CC_CUTOFF ) {
          kaossYPad = value;
          MIDI.sendControlChange(KAOSS_CC_Y, kaossYPad, kaossMidiOut);
      }
      else if (control == CC_CHANNEL_VOLUME ) {
          MIDI.sendControlChange(KAOSS_CC_VOLUME, value, kaossMidiOut);
      }
      else if (control == CC_RESONNANCE  ) {
          kaossXPad = value;
          MIDI.sendControlChange(KAOSS_CC_X, value, kaossMidiOut);
      }
      else MIDI.sendControlChange(control, value, kaossMidiOut);
}
