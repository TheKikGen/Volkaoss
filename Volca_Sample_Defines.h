// VOLKASSILATOR PROJECT
//
// Specific Vocla Sample defines /globals and functions
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// TheKikGen - Nov 2016.

// =================================================================================
// VOLCA SAMPLE DEFINES
// =================================================================================
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

#define VSAMPLE_CMD_MODE_KEY          12 // Command mode root Key - C0. Always on Channel 10.

#define VSAMPLE_CMD_TOGGLE_CC_MIDI_IN     VSAMPLE_CMD_MODE_KEY+2 // Midi IN on/off for the selected part (channel)
#define VSAMPLE_CMD_RESET_ALL_CC          VSAMPLE_CMD_MODE_KEY+4 // Reset all CC value of the current part

/*16  Gen Purpose Controller 1
17  Gen Purpose Controller 2
18  Gen Purpose Controller 3
19  Gen Purpose Controller 4*/

byte vSampleLastNoteOnChannel = 0 ;
int  vSampleccMidiInState = 0 ;   // if the (channel) bit is 1 , then MIDI IN is ON
bool vSampleCommandModeKeyPressed=false;

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

// Function prototypes

void VSampleBlinkedChannels(byte num);
void VSampleResetAllCC(byte channel);
void VSampleToggleCCMidiIn(byte channel);
void VSampleProcessNoteOff(byte channel, byte note, byte velocity);
void VSampleProcessNoteOn(byte channel, byte note, byte velocity);
void VSampleProcessControlChange(byte channel, byte control, byte value) ;
void VSampleInitialize();

