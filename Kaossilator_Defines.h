// THE VOLKASSILATOR PROJECT
//
// Specific Kaossilator pro defines
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// TheKikGen - Nov 2016.

// ====================================================================
// KAOSSILATOR PRO  DEFINES
// ====================================================================
#define KAOSS_MIDI_IN             1   // Default MIDI in channel to capture events to Kaosspad
#define KAOSS_MIDI_OUT            11  // Remember that when  using Volcasample, channel 1-10 are used
#define KAOSS_NOTE_TRANSPOSE      36
#define KAOSS_YPAD_VALUE          64
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

#define KAOSS_CMD_ALLBANKS_OFF          KAOSS_CMD_MODE_KEY + 1
#define KAOSS_CMD_BANK_A                KAOSS_CMD_MODE_KEY + 2
#define KAOSS_CMD_ALLBANKS_ON           KAOSS_CMD_MODE_KEY + 3
#define KAOSS_CMD_BANK_B                KAOSS_CMD_MODE_KEY + 4
#define KAOSS_CMD_BANK_C                KAOSS_CMD_MODE_KEY + 5
#define KAOSS_CMD_SET_MIDI_CHANNEL_OUT  KAOSS_CMD_MODE_KEY + 6
#define KAOSS_CMD_BANK_D                KAOSS_CMD_MODE_KEY + 7
#define KAOSS_CMD_GATE_ARP_SPEED_DEC    KAOSS_CMD_MODE_KEY + 8
#define KAOSS_CMD_GATE_ARP              KAOSS_CMD_MODE_KEY + 9
#define KAOSS_CMD_GATE_ARP_SPEED_INC    KAOSS_CMD_MODE_KEY +10
#define KAOSS_CMD_SET_MIDI_CHANNEL_IN   KAOSS_CMD_MODE_KEY +11


// Bank on/off status Bit mask
#define KAOSS_BANK_STATE_A        0
#define KAOSS_BANK_STATE_B        1
#define KAOSS_BANK_STATE_C        2
#define KAOSS_BANK_STATE_D        3
#define KAOSS_GATE_ARP_STATE      4

// KAOSS BANK TRIGGERS QUANTIZED ON / OFF globals
// KAOSS COMMANDS BASED ON events synchronized with the MIDI clock
byte kaossMidiIn=KAOSS_MIDI_IN;
byte kaossMidiOut=KAOSS_MIDI_OUT;
byte kaossMidiInTemp=0;
byte kaossMidiOutTemp=0;
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
byte kaossYPad  = KAOSS_YPAD_VALUE ; // Start at the middle because at 0, sometimes no sound, depending on patches
byte kaossXPadPress = 0;
byte kaossYPadPress = 0;

int  kaossNoteOn = 0 ;             // Kaoss pad note on dedicated note on counter
int  kaossNoteTranspose = kaossNoteTranspose = KAOSS_NOTE_TRANSPOSE ;     // Offset from C-1
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

// Functions prototypes
void KaossProcessProgramChange(byte channel, byte number);
void KaossProcessNoteOff(byte channel, byte note, byte velocity);
void KaossProcessNoteOn(byte channel, byte note, byte velocity);
void KaossTrigEvents();
void KaossProcessStop();
void KaossInitialize() ;
void KaossGateArpSetSpeed(bool speedIncrease);
void KaossProcessPitchBend(byte channel, int pitch);
void KaossSetAllBanks(bool banksOn) ;
void KaossToggleBank(byte ccBank, byte bankState);


