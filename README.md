# Volkaoss
VOLKAOSS -  VOLCASAMPLE AND KAOSSCILATOR AS TRUE DRUM BOX / SYNTH

Since a few months, I use more and more hardware gear and synths, and a little less frequently software like VST plugins, the urge to touch buttons, turn knobs, plug and unplug cables. Thus, my setup is becoming a patchwork of "old" analog synths, sequencing softwares, sampler, and other more or less recent gears ...

The common point of all this: MIDI (not always). And in the world of MIDI, it is sometimes complicated to talk to each other.

## The Korg Volca Sample :

I like very much this small white box capable, for a very low price, to spit sounds really percussive, if you make the effort to process samples correctly. The big catch is that it is not really usable on a MIDI chain, because it monopolizes 10 channels, one per sample part, from 1 to 10.   In addition, the samples loaded in the beast can not be played chromatically with a MIDI keyboard. To change the pitch, you have to manually adjust the Speed knob (or send the ad-hoc MIDI controller).





## The Korg Kaossilator pro 3 (KP3) :
I also have another kid from Korg, a little bit older, but, with huges sounds and an X/Y pad : the Kaossilator Pro 3. This device is also not easily integrable in a MIDI setup, simply because it does not recognize MIDI notes, but only non standard midi CC controls.  For sure, some will tell me the KP3 is a pad and is finally more a surface controller rather than a synth. I absolutely disagree !! Some patches are so clean and cool, especially bass ones, most others are good, drum kits are very dynamic, the vocoder is a nice feature, and there are still 4 banks to record and play samples.

BUT, anyone who has tried the Kaossilator knows that : playing a precise note on the PAD is almost impossible (sorry I have big fingers !!), even using the SCALE function. Second problem, the recorded samples are difficult to synchronize with the BPM because there is no Start / Stop MIDI implemented in the KP3. To start a sample, you have to press the "Bank n" button on time : it is more tricky than you think. Better to be awake ! It is a serious issue.

Despite the big potential of the KP3 unfortunately constrained by Korg's design mistakes, after a few months to play with, it finally remains in the closet, and slowly takes the dust before being sold a misery. While precisely I was thinking to sell mine, I renounced after played a "last time" with it because it sounds so good.

## The idea

I had already seen projects, quite experimental on all kinds of platforms, to map the notes in MIDI controls and to trigger the sounds of the Kaossilator. Likewise for the Volca sample, a company is selling a special MIDI cable embedding a MIDI powered microcontroller for about € 35 + delivery that allows to operate the Volca Sample with only the channel 10 / as a GM module. I watched the demo video on their website : this is close to what I imagined doing when I thought about it, and it's rather well done, but that is a little too dedicated to the Volca in my opinion...

Because developing software is a part of my job, I therefore started the development of what I called the "Volkaoss" (VK if you don't speak Klingon). I wanted to make VK a MIDI controller / converter that is both the most universal and the most economical possible... I happen to have an Arduino Uno at the bottom of a drawer!

I wanted a plug and play solution, without soldering, so I ordered an Arduino MIDI SHIELD for less than 10 euros, and after several rather intense weeks of development, I produced a firmware which runs quite well in the Arduino Uno.

I used a master MIDI keyboard and to get 2 midi outs, and simultaneously drive the Kaossilator and the Volca sample with KV , I used a "midi splitter" cable. You can find some on Ebay for few euros. You can also make one easily with 3 midi cables by connecting all RX, all TX, and all ground together.



KV receives usually on channel 1 for the KP3 and channel 10 for the Volca Sample.



It is a real pleasure to play on the KP3 with a keyboard, at the right pitch, transforming it to a very powerful mono synth at a price defying competition and to be able to use the Volca Sample as a real drum machine, managing velocity, and Compatible GM / channel 10 !!









VK features for the Kaossilator Pro 3 / Pro 3 +

Map midi notes to specific pad press and X, Y control changes (1)
Manage holded note on when playing another as a mono synth do
Pitch bend handling relatively to the note currently pressed on the keyboard
Map Cutoff, Reverb level or Modulation Wheel to PAD Y 
Map Resonnance to a totally free PAD X
Velocity sensitivity
Commands on the C0 octave  (C0 beeing the "command" key) :
Start/stop bank A, B, C, D.  (2)
All banks on / off (2)
ARP/GATE start / stop (2)
ARP speed increase / decrease (rotate from max to min and min to max)
Change Midi In channel
(1) The kaossilator must set to 4 octaves, and scale must be chromatic. The Key is usually better fixed to C minus.

(2) If a midi clock is received, the banks, and the ARG/GATE are started synchronously with the BPM



VK features for the Volca Sample :

Map notes to channel 10, C2 octave to Volca Sample parts on midi 1 to 10, and play chromatically the last part played (not used for recording) beyond C2
Commands on C0/ Channel 10 to toggle midi IN for parts (5-9) to use them for specific Volca controls or playing chromatically when recording with a DAW
Velocity sensitivity
