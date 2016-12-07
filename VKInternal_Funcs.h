// THE VOLKASSILATOR PROJECT
//
// Specific Kaossilator pro defines
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// Franck Touanen - Nov 2016.

// ====================================================================
// VK INTERNAL FUNCS
// ====================================================================

void VKSetGlobals() {
    vkMidiIn                = VKGlobals.vkMidiIn ;
    kaossMidiIn             = VKGlobals.kaossMidiIn  ;
    kaossMidiOut            = VKGlobals.kaossMidiOut ;
    kaossNoteTranspose      = VKGlobals.kaossNoteTranspose ;
    kaossYPad               = VKGlobals.kaossYPad; 
    vSampleccMidiInState    = VKGlobals.vSampleccMidiInState ; 
}


void VKStoreGlobals() {
    VKGlobals.vkMidiIn              = vkMidiIn;
    VKGlobals.kaossMidiIn           = kaossMidiIn;
    VKGlobals.kaossMidiOut          = kaossMidiOut;
    VKGlobals.kaossNoteTranspose    = kaossNoteTranspose ;
    VKGlobals.kaossYPad             = kaossYPad;
    VKGlobals.vSampleccMidiInState  = vSampleccMidiInState ;
    EEPROM.put(eeAddress, VKGlobals);
}

void VKFactoryInit(bool softReset=false) {
    for (int i = 0 ; i < EEPROM.length() ; i++) EEPROM.write(i, 0);
    strcpy( VKGlobals.sign,VKINTERNAL_SIGNATURE);
    strcpy( VKGlobals.ver, VKINTERNAL_VERSION);
    VKGlobals.vkMidiIn              = VKINTERNAL_MIDI_IN;
    VKGlobals.kaossMidiIn           = KAOSS_MIDI_IN;
    VKGlobals.kaossMidiOut          = KAOSS_MIDI_OUT;
    VKGlobals.kaossNoteTranspose    = KAOSS_NOTE_TRANSPOSE ;
    VKGlobals.kaossYPad             = KAOSS_YPAD_VALUE;
    VKGlobals.vSampleccMidiInState  = 0;
    EEPROM.put(eeAddress, VKGlobals);
    if (softReset) ArduinoSoftReset();
}

void VKShowParams() {
  Serial.println("============================================================================");
  Serial.print("Version                             : ");
  Serial.println(VKGlobals.ver);
  Serial.print("VK Midin                            : ");
  Serial.println(VKGlobals.vkMidiIn);
  Serial.print("Kaossilator Midi In                 : ");
  Serial.println(VKGlobals.kaossMidiIn);
  Serial.print("Kaossilator Midi Out                : ");
  Serial.println(VKGlobals.kaossMidiOut);
  Serial.print("Kaossilator transpose               : ");
  Serial.println(VKGlobals.kaossNoteTranspose);
  Serial.print("Kaossilator  PAD default value      : ");
  Serial.println(VKGlobals.kaossYPad);
  Serial.print("Volca Sample midi in channel status : ");
  Serial.println(VKGlobals.vSampleccMidiInState,BIN);  
  Serial.println("============================================================================");
}

void VKProcessNoteOn(byte channel, byte note, byte velocity) {
    if (note == VKINTERNAL_CMD_MODE_KEY) vkCommandModeKeyPressed=true;
    else if ( vkCommandModeKeyPressed ) {
            // Initialize parameters in EEPROM (reset to default)
            if (note == VKINTERNAL_CMD_FACTORY_INIT) VKFactoryInit(true);
            // Store parameters into EEPROM
            else if (note == VKINTERNAL_CMD_SAVE) VKStoreGlobals();
    }
}

void VKProcessNoteOff(byte channel, byte note, byte velocity) {
  if (note == VKINTERNAL_CMD_MODE_KEY) vkCommandModeKeyPressed=false;
}


