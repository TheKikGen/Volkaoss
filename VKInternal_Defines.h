// THE VOLKASSILATOR PROJECT
//
// Specific Kaossilator pro defines
//
// - Allow to play MIDI notes with the Kaossilator pro synth
// - Allow to map Volcal Sample parts 1-10 to a GM standard rythm channel 10
// Franck Touanen - Nov 2016.

// ====================================================================
// VK INTERNAL DEFINES
// ====================================================================

#define VKINTERNAL_SIGNATURE "VK06"
#define VKINTERNAL_VERSION   "0.00"
#define VKINTERNAL_MIDI_IN 16

#define VKINTERNAL_CMD_MODE_KEY         12 // Command mode C0
#define VKINTERNAL_CMD_SAVE             VKINTERNAL_CMD_MODE_KEY+2 
#define VKINTERNAL_CMD_FACTORY_INIT     VKINTERNAL_CMD_MODE_KEY+4
#define VKINTERNAL_CMD_TOGGLE_DEBUG     VKINTERNAL_CMD_MODE_KEY+5
#define VKINTERNAL_CMD_SOFT_RESET       VKINTERNAL_CMD_MODE_KEY+7

#define VKINTERNAL_MODE_ALL      00  // All functions
#define VKINTERNAL_MODE_KAOSS    01  // Kaossilator only
#define VKINTERNAL_MODE_VOLCA    02  // Volcasample Only

// Settings structure. Used to store parameters in the EEPROM of the Arduino. 512 bytes max.

struct VKSettingsTemplate {
        char sign[5];
        char ver[5];
        bool debugMode;
        byte vkMidiIn;
        byte kaossMidiIn;
        byte kaossMidiOut;
        byte kaossNoteTranspose;
        byte kaossYPad;
        int  vSampleccMidiInState;
} VKGlobals, VKeeGlobals;
int           eeAddress = 0; // EEPROM address offset

byte vkMidiIn=VKINTERNAL_MIDI_IN;
bool vkCommandModeKeyPressed=false;
byte vkMode = VKINTERNAL_MODE_ALL;

// Functions prototypes
void VKSetGlobals();
void VKStoreGlobals();
void VKFactoryInit(bool softReset=false);
void VKShowParams();
void VKProcessNoteOn(byte channel, byte note, byte velocity);
void VKProcessNoteOff(byte channel, byte note, byte velocity);

