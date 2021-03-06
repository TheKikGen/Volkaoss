# Volkaoss 
VOLCASAMPLE AND KAOSSCILATOR AS TRUE DRUM BOX / SYNTH

<img width="250" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/Volkaoss_paperbox.jpg?raw=true"  /> <img width="250" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/arduino_uno.jpg?raw=true"  />

I use more and more hardware gear and synths, and a little less frequently software like VST plugins, the urge to touch buttons, turn knobs, plug and unplug cables. Thus, my setup is becoming a patchwork of "old" analog synths, sequencing softwares, sampler, and other more or less recent gears ... 
The common point of all this: MIDI (not always). And in the world of MIDI, it is sometimes complicated to talk to each other.

"Volkaoss" (VK if you don't speak Klingon) is a MIDI controller / converter allowing you to play MIDI notes on your Kaossilator Pro 3 as a mono synth, and to use the Volca Sample as a GM drum machine on the MIDI channel 10.   This is a totally plug and play solution, without soldering, based on the famous Arduino platform.

## The Korg Volca Sample :

I like very much this small white box capable, for a very low price, to spit sounds really percussive, if you make the effort to process samples correctly. The big catch is that it is not really usable on a MIDI chain, because it monopolizes 10 channels, one per sample part, from 1 to 10.   In addition, the samples loaded in the beast can not be played chromatically with a MIDI keyboard. To change the pitch, you have to manually adjust the Speed knob (or send the ad-hoc MIDI controller).

## The Korg Kaossilator pro 3 (KP3) :
I also have another kid from Korg, a little bit older, but, with huges sounds and an X/Y pad : the Kaossilator Pro 3. This device is also not easily integrable in a MIDI setup, simply because it does not recognize MIDI notes, but only non standard midi CC controls.  For sure, some will tell me the KP3 is a pad and is finally more a surface controller rather than a synth. I absolutely disagree !! Some patches are so clean and cool, especially bass ones, most others are good, drum kits are very dynamic, the vocoder is a nice feature, and there are still 4 banks to record and play samples.

BUT, anyone who has tried the Kaossilator knows that : playing a precise note on the PAD is almost impossible (sorry I have big fingers !!), even using the SCALE function. Second problem, the recorded samples are difficult to synchronize with the BPM because there is no Start / Stop MIDI implemented in the KP3. To start a sample, you have to press the "Bank n" button on time : it is more tricky than you think. Better to be awake ! 

## The project

I had already seen projects, quite experimental on all kinds of platforms, to map the notes in MIDI controls and to trigger the sounds of the Kaossilator. Likewise for the Volca sample, a company is selling a special MIDI cable embedding a MIDI powered microcontroller for about € 35 + delivery that allows to operate the Volca Sample with only the channel 10 / as a GM module. I watched the demo video on their website : this is close to what I imagined doing when I thought about it, and it's rather well done, but that is a little too dedicated to the Volca in my opinion...

Because developing software is a part of my job, I therefore started the development of what I called the "Volkaoss" (VK if you don't speak Klingon). I wanted a cheap plug and play solution, with no soldering, so I ordered an Arduino+ a MIDI SHIELD for less than 10 euros, and after several rather intense weeks of development, I produced a firmware which runs quite well in the Arduino Uno.

It is a real pleasure to play on the KP3 with a keyboard, at the right pitch, transforming it to a very powerful mono romsynth at a price defying competition and to be able to use the Volca Sample as a real drum machine, managing velocity, and Compatible GM / channel 10 !!

## Quick setup :

### Arduino hardware
You can search on Ebay for "Aduino Uno" and "Arduino MIDI SHIELD".
For example : Arduino Uno from UK  and Arduino Midi shield from HK .
It should cost less than 15 €, shipping included.
You simply have to put the Midi shield into the Arduino Uno socket. That's all you have to do !!

<img width="250" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/midi_shield.JPG?raw=true"  /> <img width="250" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/midishield2.jpg?raw=true"  /> 

## Firmware (sketch) download and uploading to the Arduino.
Download the Volkaoss project hre :https://github.com/TheKikGen/Volkaoss/archive/master.zip and unzip all files to a Volkaoss directory. Install Arduino IDE, and plug your Arduino Uno board to the USB port, then compile and upload the firmware. 

## MIDI setup
The kaossilator must be set to 4 octaves, scale must be chromatic.  
The Key is usually better fixed to C minus. 
The receiving MIDI channel is 11 by default. It must be set to 11 in Midi / MSSG / GL.CH else the koassilator will not receive messages from Volkaoss.  

The bank A,B,C,D buttons must be configured to  C2, C#2, D2, D#2.  This is the default configuration.
Kaossilator pro Control change # are (corresponding to Kaossilator default factory) :
* 92  pad on/off control change
* 12  pad X control change
* 13  pad Y control change
* 90  GATE ARP on/off control change
* 91  GATE ARP speed Control change
* 93  GATE ARP time  Control change
You can change these in the Kaossilator Midi setting menu.

TIPS : If you want to store permanently your Kaossilator configuration,  set your key, scale, and range , and store them in to a program bank.  Then save programs to bank 0, and again ALL to bank 0. At the next boot, you kaossilator will be ready to use with Volkaoss.

The firmware covers both Kaossilator Pro 3 / Pro 3+ and the Volca Sample. You need a splitter cable to control the 2 devices at the same time. Otherwise, simply use a classic midi cable if you own only one of these devices.

<img width="500" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/volkaoss_setup.png?raw=true"  /> 

* Plug the MIDI OUT of your keyboard to the Midi IN of the Volkaos. 
* Plug the splitter cable to the MIDI Out of the Volkaoss.
* Plug first out of the splitter to the Midin In of the Kaossilator pro, and the second one to the Midi IN of the Volcasample.
* Plug a +5V power supply or a USB cable to the Arduino board, then choose some samples on parts 1-10 of the Volca Sample.

## Testing
On the kaossilator, choose the piano patch (A42), and send notes to the channel 1, beyond C1. You should hear a piano sound and see the green light on the pad. Send notes from C2 to the channel 10 : you should hear the Volca sample sounds.

## VK global commands
You can use the following commands on the **channel 16**, dedicated to VK configuration.: 

<img width="300" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/volkaoss_vk_commands.png?raw=true"  /> 

* Save current configuration permanently to the EEPROM
* Reset to factory default : Midin 1, Out 11, transpose 36, PAD Y 64 for the Kaossilator. No activated midi in channels for the Volca sample. Debug mode off.
* Toggle Debug : activate/ inactivate debug mode. The debug mode sends messages to a terminal connected to the serial port at 115200 bauds.  If you need to boot with debug mode activated, you must save the current configuration.  When debug is on , your midi equipment could react to what is sent to the serial port, especially at boot time, because midi out and terminal are sharing the same serial port.  So don't forget to set debug OFF in normal conditions.
* Soft reset : reboot Volkaoss (soft reboot is not exactly the same as pushing the reset button).  You can use this command to apply what you saved, or simply to reset the Volkaoss remotely from your MIDI keyboard when necessary.

      Debug mode display at boot time on terminal, 115200 bauds :
      VOLKAOSS - MULTI-DEVICES MIDI CONTROLLER
      Build number : 1.071216.0040
      ================================================
      Parameters retrieved from EEPROM
      ================================================
      Version                             : 0.00
      Debug Mode                          : 1
      VK Midin                            : 16
      Kaossilator Midi In                 : 7
      Kaossilator Midi Out                : 11
      Kaossilator transpose               : 36
      Kaossilator  PAD default value      : 64
      Volca Sample midi in channel status : 0
      ================================================

## VK for the Kaossilator Pro 3 / Pro 3 +

* Map midi notes to specific pad press and X, Y control changes 
* Manage holded note on while playing another as a mono synth do 
* Pitch bend handling relatively to the note currently pressed on the keyboard 
* Map Cutoff, Reverb level or Modulation Wheel to PAD Y 
* Map Resonance to a totally free PAD X 
* Velocity sensitivity 
* Commands on the C0 octave (C0 beeing the "command key") : 
* Start/stop bank A, B, C, D. 
* All banks on / off (2) 
* ARP/GATE start / stop 
* ARP speed increase / decrease (cyclic) 
* Change Midi In channel (between 1-15.  Beyond 15 stays at 1 whatever the #)
* Change Midi out channel

<img width="500" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/volkaoss_kaoss_commands.png?raw=true"  /> 

To enter a command, you must hold both the command key (C0) and the key corresponding to the function you need to use. If a midi clock is received, the banks, and the ARG/GATE are started synchronously with the BPM. The ARP/gate speed increase/decrease are cyclics.
If you need to change the midi in channel for the Volkaoss, you must tap the "Change midi in key " a number of time corresponding to the channel number.  Beyond 16, it stays at 1. The channel will change only after you release the command key. So, don't forget to adjust the midi out channel in your midi keyboard.  Same behaviour for the Midi out change.

## VK for the Volca Sample :
* Map notes on channel 10, C2 octave to Volca Sample parts on midi 1 to 10, and play chromatically the last part played (not used for recording) beyond C2 
* Velocity sensitivity 
* Commands on C0 to toggle midi IN for parts (5-9) to use them for specific Volca controls or playing chromatically when recording with a DAW 
* Reset controllers values 

<img width="300" border="0" src="https://github.com/TheKikGen/Volkaoss/blob/master/doc/volkaoss_volca_commands.png?raw=true"  /> 

Notes are starting with part 1 at C2, part 2 at C#2, etc... on the channel 10 (as the General Midi drums standard). All commands must be sent from the channel 10, at the C0 octave.

Enabling MIDI IN channels will allow you to tweak part in real time, like the knobs of the Volca Sample. Only channels from 5 to 9 can be enabled. To activate/inactivate (toggle) the part midi in channel, you must first select it, by emitting the note on the channel 10 corresponding to that part. For example, if you need to tweak a Kick drum, choose a sample at the part #5, press the corresponding note on the midi keyboard (E2) at the channel 10, then Command key + Toogle Midi In.  You just enabled the midi in on channel 5.  Then use your midi controller to tweak values.

The "Reset all CC" command will initialize all CC values to their original value for the selected part.

## Issues reporting

Use the GitHub issue tab.
