# MIDI Mod-Mate
This is a MIDI effect (can be inserted in the MIDI effects slot of Apple Logic Pro X) to provide a very basic MIDI "macro" capability.

I developed this specifically for use with [Airwave V3 for Omnisphere](https://www.pluginguru.com/products/airwave-v3/), a pair of sound libraries for [Spectrasonics Omnisphere](https://www.spectrasonics.net/products/omnisphere/) developed by Belgian DJ and sound designer Laurent V&eacute;ronnez (aka *Airwave*) and John "Skippy" Lemkuhl aka [PlugInGuru.com](https://www.pluginguru.com/).

The *Airwave V3* sound libraries are unique in that four distinct MIDI CCs (continuous controllers) can be used to affect the sound in real-time:
 * CC#1 (mod wheel)
 * CC#2 (breath controller)
 * CC#4 (foot controller)
 * CC#67 (soft pedal)

For the very best experience, you should use a hardware MIDI controller which provides multiple MIDI CCs, preferably in the form of touch sliders, as on the excellent [Maschine JAM](https://www.native-instruments.com/en/products/maschine/production-systems/maschine-jam/), or [Sonic Logic 3 for iPad](https://www.soniclogicapps.com/). For people who are not yet ready or able to buy these very nice hardware MIDI systems, I created this very simple plug-in, which lets you at least get started with *Airwave V3*, using just the standard controls available on nearly all MIDI keyboards: pitch bend and mod wheel.

## Using the ModMate plug-in
![](modmate.png)
The four large rectangles on the right of the GUI show the output levels for MIDI CC's 1, 2, 4, and 67 (top to bottom). The three narrow vertical rectangles on the left show the input levels (from your MIDI controller) for pitch-bend up (pbUp), pitch-bend down (pbDn), and mod wheel (modW). To the right of each of these indicator strips are four checkboxes; checking any box connects that input to the corresponding CC output. Try it; it's easier to use than to explain.

If your DAW supports it, your selections on the 12 checkboxes can be saved as a preset, and will normally be saved automatically as part of the track settings in a saved project. This has been tested with the Mac AU version in *Logic Pro X* and with the VST version in [Reaper](https://www.reaper.fm/index.php) 64-bit.

## Obtaining the plug-in
This is *free, open-source software*. You can build it yourself from the source code here; see below under Acknowledgements.

If you can't or don't want to build these plug-ins from source, pre-built version are available at http://getdunne.net/download/modmate.

## Installing and using on a Mac
**Audio Unit version:** Copy the file *ModMate.component* to the your *~/Library/Audio/Plug-Ins/Components*. On recent versions of macOS you **must** log-out and then back in again (or restart your Mac) before the ModMate plug-in will show up in DAWs such as *Logic Pro X*.

**VST and VST3:** If your DAW requires VST plug-ins, copy the file *ModMate.vst* to *~/Library/Audio/Plug-Ins/VST* and/or the file *ModMate.vst3* to *~/Library/Audio/Plug-Ins/VST3*, then restart your Mac before opening your DAW.

## Installing and using the VST versions on a Windows PC
Copy the appropriate *.dll* (VST2) or *.vst3* file to whatever folder your DAW uses to find plug-ins. Windows DAWs vary a lot; refer to the manual for your specific DAW. If your DAW is 64-bit (nearly all recent ones are), use a 64-bit version of the plug-in; use the 32-bit version only if your DAW is an older 32-bit program.

Note that ModMade is a *MIDI Effect plug-in*, which needs to be inserted so it processes the incoming MIDI data before it reaches any VSTi instrument plug-ins on the same track, unlike regular audio-effect plug-ins are connected to the VSTs' *outputs*.

Windows DAWs all have different ways to set this up, and there are a few that don't support MIDI effects at all, so please check the manual for your specific DAW. In *Reaper*, you simply have to ensure that the ModMate comes first in whatever list of plug-ins you put on a track.

## Acknowledgements

![](VST2LogoWhite.jpg "VST2 logo") ![](VST3LogoBlack.jpg "VST3 logo")

*VST* and its expansion *Virtual Studio Technologies* are registered trade marks of [Steinberg Media Technologies GmbH](https://www.steinberg.net/) and are used under license.

---

![](JUCE-logo.png) ![](gplv3-with-text-136x68.png)

This program was developed with [JUCE 5.x](https://shop.juce.com/get-juce), hence subject to the [GNU GPL v3 license](https://www.gnu.org/licenses/gpl-3.0.en.html). Use the *Projucer* to generate the necessary projects, e.g. VST2 for Windows, Audio Unit for Mac.
