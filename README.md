# MIDI Mod-Mate
This is a MIDI effect (can be inserted in the MIDI effects slot of Apple Logic Pro X) to provide a very basic MIDI "macro" capability.

I developed this specifically for use with [Airwave V3 for Omnisphere](https://www.pluginguru.com/products/airwave-v3/), a pair of sound libraries for [Spectrasonics Omnisphere](https://www.spectrasonics.net/products/omnisphere/) developed by Belgian DJ and sound designer Laurent V&eacute;ronnez (aka *Airwave*) and John "Skippy" Lemkuhl aka [PlugInGuru.com](https://www.pluginguru.com/).

The *Airwave V3* sound libraries are unique in that four distinct MIDI CCs (continuous controllers) can be used to affect the sound in real-time:
 * CC#1 (mod wheel)
 * CC#2 (breath controller)
 * CC#4 (foot controller)
 * CC#67 (soft pedal)

For the very best experience, you should use a hardware MIDI controller which provides multiple MIDI CCs, preferably in the form of touch sliders, as on the excellent [Maschine JAM](https://www.native-instruments.com/en/products/maschine/production-systems/maschine-jam/), or [Sonic Logic 3 for iPad](https://www.soniclogicapps.com/). For people who are not yet ready or able to buy these very nice hardware MIDI systems, I created this very simple plug-in, which lets you at least get started with *Airwave V3*, using just the standard controls available on nearly all MIDI keyboards: pitch bend and mod wheel.

## Using the ModMate plug-in (basic)
![](modmate.png)

The four large rectangles on the right of the GUI show the output levels for MIDI CC's 1, 2, 4, and 67 (top to bottom). The six narrow vertical rectangles on the left show the input levels (from your MIDI controller) for pitch-bend up (pbUp), pitch-bend down (pbDn), and mod wheel (modW), as well as MIDI CCs 2, 4, and 67 (ignore these if your MIDI controller has only pitch and mod wheels). To the right of each of these indicator strips are four checkboxes; checking any box connects that input to the corresponding CC output. Try it; it's easier to use than to explain.

If you don't check any of an input control's four destination boxes, the control's data will be passed through the plug-in unchanged, so you can use that control normally.

If your DAW supports it, your selections on all the checkboxes can be saved as a preset, and will normally be saved automatically as part of the track settings in a saved project. This has been tested with the Mac AU version in *Logic Pro X* and with the VST version in [Reaper](https://www.reaper.fm/index.php) 64-bit.

## Advanced features
You can **click/drag in any of the vertical indicators** on the left, and the plug-in will respond as though you had operated the corresponding input CC, and will generate CC output messages for whichever output CCs are checked for that input. You can also **click/drag in any of the horizontal indicators on the right**, and the plug-in will generate the corresponding output CC messages.

You can **double-click any of the *colored* slider labels** (not *pbUp* or *pbDn*) to change the corresponding MIDI CC number. You may type any number between 1 and 127; you don't have to put "cc" at the start, but it's OK if you do. You must press *Enter* to confirm the change. Note CC#1 will always display as "modW" because MIDI CC #1 *is* the standard mod wheel. Alternatively, you can  **right-click any colored slider label** to choose the CC number from a list of standard controller names, e.g. "Breath Controller".

If you have a MIDI controller with knobs or sliders pre-assigned to MIDI CC numbers other than those used by the *Airwave V3* libraries, you'll want to change the *input* CC numbers along the top of the plug-in window, to whatever CC's your controller outputs.

If you're using *ModMate* with an software instrument or library which is set up to respond to MIDI CCs other than what the *Airwave V3* libraries use, you'll want to change the *output* CC numbers to the left of each large horizontal indicator.

If your DAW supports it, your MIDI CC assignments can be saved/restored in presets, along with the checkbox settings.

## Obtaining the plug-in
This is *free, open-source software*. You can build it yourself from the source code here; see below under Acknowledgements.

If you can't or don't want to build these plug-ins from source, pre-built version are available at http://getdunne.net/download/modmate.

## Installing and using on a Mac
**Audio Unit version:** Copy the file *ModMate.component* to the your *~/Library/Audio/Plug-Ins/Components*. On recent versions of macOS you **must** log-out and then back in again (or restart your Mac) before the ModMate plug-in will show up in DAWs such as *Logic Pro X*.

**VST and VST3:** If your DAW requires VST plug-ins, copy the file *ModMate.vst* to *~/Library/Audio/Plug-Ins/VST* and/or the file *ModMate.vst3* to *~/Library/Audio/Plug-Ins/VST3*, then restart your Mac before opening your DAW.

## Installing and using the VST versions on a Windows PC
Copy the appropriate *.dll* (VST2) or *.vst3* file to whatever folder your DAW uses to find plug-ins. Windows DAWs vary a lot; refer to the manual for your specific DAW. If your DAW is 64-bit (nearly all recent ones are), use a 64-bit version of the plug-in; use the 32-bit version only if your DAW is an older 32-bit program.

Note that ModMade is a *MIDI Effect plug-in*, which needs to be inserted so it processes the incoming MIDI data before it reaches any VSTi instrument plug-ins on the same track, unlike regular audio-effect plug-ins are connected to the VSTs' *outputs*.

Windows DAWs all have different ways to set this up, and there are a few that don't support MIDI effects at all, so please check the manual for your specific DAW. In *Reaper*, you simply have to ensure that the ModMate comes first in whatever list of plug-ins you put on a track. Unfortunately, *Cubase* users are out of luck, as *Cubase* still does NOT support third-party MIDI plug-ins; only their own.

## Acknowledgements

![](VST2LogoWhite.jpg "VST2 logo") ![](VST3LogoBlack.jpg "VST3 logo")

*VST* is a trademark and software of [Steinberg Media Technologies GmbH](https://www.steinberg.net/).

---

![](JUCE-logo.png) ![](gplv3-with-text-136x68.png)

This program was developed with [JUCE 5.x](https://shop.juce.com/get-juce), hence subject to the [GNU GPL v3 license](https://www.gnu.org/licenses/gpl-3.0.en.html). Use the *Projucer* to generate the necessary projects, e.g. VST2 for Windows, Audio Unit for Mac. To build VST/VST3 you'll need the [VST3 SDK](https://github.com/steinbergmedia/vst3sdk) from Steinberg. To distribute finished products, you may need a [license](https://www.steinberg.net/en/company/developers.html).
