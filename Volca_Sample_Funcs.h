// VOLKASSILATOR PROJECT
//
// Specific Vocla Sample defines /globals and functions
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// TheKikGen - Nov 2016..

// VOLCA FUNCS========================================================================

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

void VSampleProcessNoteOff(byte channel, byte note, byte velocity) {
  if (note == VSAMPLE_CMD_MODE_KEY) vSampleCommandModeKeyPressed=false;
}

void VSampleProcessNoteOn(byte channel, byte note, byte velocity) {
  static int thisVar;

  // Remind : CMD Only on the channel 10
  if (channel == VSAMPLE_MIDI_IN ) {
    if (note == VSAMPLE_CMD_MODE_KEY) vSampleCommandModeKeyPressed=true;
    else if ( vSampleCommandModeKeyPressed ) {
            // Reset all CC on the current part
            if ( note == VSAMPLE_CMD_RESET_ALL_CC && vSampleLastNoteOnChannel >0) VSampleResetAllCC(vSampleLastNoteOnChannel);
            // Midi IN  channel ON / OFF
            else if ( note == VSAMPLE_CMD_TOGGLE_CC_MIDI_IN && vSampleLastNoteOnChannel >0 )  VSampleToggleCCMidiIn(vSampleLastNoteOnChannel);
            // Play note in GM Mode
    }
    
    // Dispatch notes to Volcal Sample channels
    if ( note >= VSAMPLE_ROOT_GM_KEY && note < (VSAMPLE_ROOT_GM_KEY + 10)  ) {
          thisVar = constrain( note - VSAMPLE_ROOT_GM_KEY +1,0,127);
          MIDI.sendControlChange(CC_CHANNEL_VOLUME, velocity, thisVar);  // Velocity simulation
          MIDI.sendNoteOn(VSAMPLE_ROOT_GM_KEY,127, thisVar);
          vSampleLastNoteOnChannel = thisVar ;
    }
    // Play note chromatically in GM mode FROM ROOT CHROMATIQUE KEY on the channel 10
    else if ( note >= VSAMPLE_ROOT_CHROMATIC_KEY && note <= 127 && vSampleLastNoteOnChannel ) {
          MIDI.sendControlChange(CC_CHANNEL_VOLUME, velocity, vSampleLastNoteOnChannel);  // Velocity simulation
          MIDI.sendControlChange(VSAMPLE_CC_SPEED, VSAMPLE_NOTE_TO_CC_SPEED[note], vSampleLastNoteOnChannel );
          MIDI.sendNoteOn(VSAMPLE_ROOT_CHROMATIC_KEY,127, vSampleLastNoteOnChannel);
    }
  }
  // Play notes chromatically on an activated channel (for MIDI recording purpose) other than 10
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
