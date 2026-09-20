# THE GOAT

By THE EDGE OF FEAR

The ultimate Swedish Chainsaw and Modern Deathcore HM-2 cult distortion plugin. Featuring dual-gyrator tone stack emulation, asymmetric Germanium/Silicon/LED diode saturation, 4x polyphase oversampling, and parallel clean blend.

---

## Plugin Overview

THE GOAT by THE EDGE OF FEAR is a boutique digital emulation of the legendary Japanese 1980s Boss HM-2 Heavy Metal circuit and the Melbourne-crafted Australian boutique evolution, the Kink Pedals "Oath of the Goat".

THE GOAT accurately models the entire multi-stage analog signal path: from the 72.3 Hz Butterworth pre-emphasis filter and discrete transistor buffer treble shelf, through the 4x oversampled asymmetric diode clipping engine, into the passive 530 Hz mid-scoop, and across the iconic active Dual-Gyrator Tone Stack (100 Hz Low and 1.3 kHz Buzzsaw Core).

---

## Documentation and User Guides

* [Pedal Description and DSP Architecture](DESCRIPTION.md): Comprehensive circuit analysis, block diagrams, mathematical diode transfer functions, and filter equations.
* [Installation Guide](INSTALLATION.md): Step-by-step setup for VST3 (including 1-click batch installer), Standalone Desktop EXE, and DLL across Reaper, Cubase, Ableton, FL Studio, Studio One, and more.
* [User Manual and Instruction Guide](MANUAL_AND_HOW_TO_USE.md): Complete control reference, preset bank guide, MIDI Learn mapping instructions, and pro dialed-in Deathcore/Metal settings.

---

## Formats and Directory Structure

The repository provides ready-to-use release binaries in dedicated folders:

* **/VST3** (VST3 Plugin): Standard 64-bit `.vst3` bundle for DAWs + 1-click Windows elevated installer script (`INSTALL_THE_GOAT_VST3.bat`).
* **/Standalone** (Standalone App): Native 64-bit Windows executable (`THE GOAT.exe`) for live guitar playing without a DAW (ASIO low-latency support).
* **/DLL** (Standalone DLL): Direct PE 64-bit dynamic library (`THE GOAT.dll`) for modular hosts and plugin wrappers.

---

## Key Highlights and DSP Features

* Iconic Dual-Gyrator Tone Stack ("The Swedish Chainsaw Engine"):
* Low Gyrator (100 Hz): 2nd-order active peaking bell EQ (Q = 2.1, +/- 18 dB, default: +18 dB Dime).
* High Gyrator (1.3 kHz Buzzsaw Core): 2nd-order high-Q active peaking bell EQ (Q = 3.5, +/- 20 dB, default: +20 dB Dime).
* Smooth parameter interpolation via juce::SmoothedValue guarantees zero zipper noise during sweeps.


* Dual-Mode Diode Clipping Engine:
* VINTAGE HM-2: Recreates the classic Silicon + Germanium asymmetric diode pair for raw, heavily compressed, razor-sharp Swedish death metal grind.
* GOAT HOT-ROD: Red LED + Silicon asymmetric clipping with extended headroom, explosive dynamic punch, and harmonically rich bite tailored for modern Deathcore 7/8-string breakdowns.


* 4x Polyphase Half-Band IIR Oversampling:
* High-precision anti-aliasing engine with switchable 1x, 2x, 4x, and 8x factors, suppressing non-linear harmonic reflections below -80 dB.


* Passive Fixed Mid-Scoop Filter:
* 530 Hz notch filter (Q = 0.8, -6.5 dB) replicating the passive RC bridge between distortion and active tone control.


* Pre-Emphasis and Buffer Simulation:
* 20 Hz DC blocker, 72.3 Hz Butterworth high-pass filter to eliminate sub-bass mud, and +2.0 dB treble shelf at 1.8 kHz.


* Parallel Clean Blend (0% to 100% Wet):
* Parallel dry/wet blend knob for metal bassists and technical guitarists who need unclipped low-end clarity blended with brutal buzzsaw top-end.


* 22+ Curated Factory Presets:
* Built-in presets covering Swedish Death Metal, Deathcore & Modern Metal, Grindcore & Powerviolence, Doom & Sludge, and Bass Chainsaw.


* Comprehensive MIDI Learn and CC Mapping:
* Map any knob, switch, or footswitch to MIDI CC, expression pedals, or foot switches via the built-in modal editor.


* Click-Free True Bypass:
* Exponential crossfader ensures seamless switching with zero pops or clicks, complete with an illuminating blood-red LED jewel indicator.



---

Stay RAD Metal Heads... The Edge Of Fear

* **YouTube Channel**: [https://www.youtube.com/@theedgeoffearmetal](https://www.youtube.com/@theedgeoffearmetal)
* **Instagram**: [https://www.instagram.com/theedgeoffear/](https://www.instagram.com/theedgeoffear/)
* **Soundcloud**: [https://soundcloud.com/user-290758847](https://soundcloud.com/user-290758847)

## License

This project is open-source and licensed under the GNU General Public License v3.0 (GPLv3). See the LICENSE file for details.

---

## Credits

Designed, circuit-modeled, and developed by The Edge of Fear.
