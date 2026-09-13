# THE GOAT ⚡🐐
### *By THE EDGE OF FEAR*

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-black.svg)]()
[![Format](https://img.shields.io/badge/Format-VST3%20%7C%20Standalone%20%7C%20DLL-red.svg)]()
[![Framework](https://img.shields.io/badge/DSP-JUCE%208%20%2F%20C%2B%2B20-orange.svg)]()

> **The ultimate Swedish Chainsaw & Modern Deathcore HM-2 cult distortion plugin. Featuring dual-gyrator tone stack emulation, asymmetric Germanium/Silicon/LED diode saturation, 4x polyphase oversampling, and parallel clean blend.**

---

## 📸 Plugin Overview

![THE GOAT Main Interface](Assets/THE%20GOAT.png)

**THE GOAT** by **THE EDGE OF FEAR** is a boutique digital emulation of the legendary Japanese 1980s **Boss HM-2 Heavy Metal** circuit and the Melbourne-crafted Australian boutique evolution, the **Kink Pedals "Oath of the Goat"**.

Engineered from the ground up in modern C++20 / JUCE 8, **THE GOAT** accurately models the entire multi-stage analog signal path: from the $72.3\text{ Hz}$ Butterworth pre-emphasis filter and discrete transistor buffer treble shelf, through the 4x oversampled asymmetric diode clipping engine, into the passive $530\text{ Hz}$ mid-scoop, and across the iconic active **Dual-Gyrator Tone Stack** ($100\text{ Hz}$ Low & $1.3\text{ kHz}$ Buzzsaw Core).

---

## 📚 Documentation & User Guides

- 📖 **[Pedal Description & DSP Architecture](DESCRIPTION.md)** — Comprehensive circuit analysis, block diagrams, mathematical diode transfer functions, and filter equations.
- 🚀 **[Installation Guide](INSTALLATION.md)** — Step-by-step setup for VST3 (including 1-click batch installer), Standalone Desktop EXE, and DLL across Reaper, Cubase, Ableton, FL Studio, Studio One, and more.
- 🎛️ **[User Manual & Instruction Guide](MANUAL_AND_HOW_TO_USE.md)** — Complete control reference, preset bank guide, MIDI Learn mapping instructions, and pro dialed-in Deathcore/Metal settings.

---

## 📦 Formats & Directory Structure

The repository provides ready-to-use binaries and complete source code in dedicated folders:

| Directory | Format | Description |
| :--- | :--- | :--- |
| [/VST3](VST3/) | **VST3 Plugin** | Standard 64-bit `.vst3` bundle for DAWs + 1-click Windows elevated installer script (`INSTALL_THE_GOAT_VST3.bat`). |
| [/Standalone](Standalone/) | **Standalone App** | Native 64-bit Windows executable (`THE GOAT.exe`) for live guitar playing without a DAW (ASIO low-latency support). |
| [/DLL](DLL/) | **Standalone DLL** | Direct PE 64-bit dynamic library (`THE GOAT.dll`) for modular hosts and plugin wrappers. |
| [/Source](Source/) | **C++ / JUCE Source** | Complete source code, DSP chains, look-and-feel classes, and CMake project definitions. |
| [/Assets](Assets/) | **Graphics & UI** | Full-resolution interface artwork (`THE GOAT.png`, `THE GOAT BG.jpg`). |

---

## ⚡ Key Highlights & DSP Features

- **Iconic Dual-Gyrator Tone Stack ("The Swedish Chainsaw Engine")**:
  - **Low Gyrator (100 Hz)**: 2nd-order active peaking bell EQ ($Q = 2.1$, $\pm 18\text{ dB}$, default: $+18\text{ dB}$ Dime).
  - **High Gyrator (1.3 kHz Buzzsaw Core)**: 2nd-order high-$Q$ active peaking bell EQ ($Q = 3.5$, $\pm 20\text{ dB}$, default: $+20\text{ dB}$ Dime).
  - Smooth parameter interpolation via `juce::SmoothedValue` guarantees zero zipper noise during sweeps.
- **Dual-Mode Diode Clipping Engine**:
  - **VINTAGE HM-2**: Recreates the classic Silicon + Germanium asymmetric diode pair for raw, heavily compressed, razor-sharp Swedish death metal grind.
  - **GOAT HOT-ROD**: Red LED + Silicon asymmetric clipping with extended headroom, explosive dynamic punch, and harmonically rich bite tailored for modern Deathcore 7/8-string breakdowns.
- **4x Polyphase Half-Band IIR Oversampling**:
  - High-precision anti-aliasing engine with switchable 1x, 2x, 4x, and 8x factors, suppressing non-linear harmonic reflections below $-80\text{ dB}$.
- **Passive Fixed Mid-Scoop Filter**:
  - $530\text{ Hz}$ notch filter ($Q = 0.8$, $-6.5\text{ dB}$) replicating the passive RC bridge between distortion and active tone control.
- **Pre-Emphasis & Buffer Simulation**:
  - $20\text{ Hz}$ DC blocker, $72.3\text{ Hz}$ Butterworth high-pass filter to eliminate sub-bass mud, and $+2.0\text{ dB}$ treble shelf at $1.8\text{ kHz}$.
- **Parallel Clean Blend (0% to 100% Wet)**:
  - Parallel dry/wet blend knob for metal bassists and technical guitarists who need unclipped low-end clarity blended with brutal buzzsaw top-end.
- **22+ Curated Factory Presets**:
  - Built-in presets covering *Swedish Death Metal*, *Deathcore & Modern Metal*, *Grindcore & Powerviolence*, *Doom & Sludge*, and *Bass Chainsaw*.
- **Comprehensive MIDI Learn & CC Mapping**:
  - Map any knob, switch, or footswitch to MIDI CC, expression pedals, or foot switches via the built-in modal editor.
- **Click-Free True Bypass**:
  - Exponential crossfader ensures seamless switching with zero pops or clicks, complete with an illuminating blood-red LED jewel indicator.

---

## 🔧 Building from Source

To compile **THE GOAT** yourself:

### Prerequisites:
- CMake 3.22 or later
- Visual Studio 2022 (with "Desktop development with C++" workload)
- Windows 10 / 11 (64-bit)

### Build Steps:
```powershell
# Clone the repository
git clone https://github.com/TheEdgeOfFear/THE-GOAT-DSP-GUITAR-PLUGIN.git
cd THE-GOAT-DSP-GUITAR-PLUGIN

# Configure CMake
cmake -B build -S .

# Build VST3 and Standalone Release binaries
cmake --build build --config Release --target TheGoat_VST3 TheGoat_Standalone
```
The compiled binaries will be output to:
- `build/THE GOAT/TheGoat_artefacts/Release/VST3/THE GOAT.vst3`
- `build/THE GOAT/TheGoat_artefacts/Release/Standalone/THE GOAT.exe`

---

All the raw files are supplied for your adjustments and builds - stay RAD Metal Heads... The Edge Of Fear 
Youtube Channel - https://www.youtube.com/@theedgeoffearmetal
Instagram - https://www.instagram.com/theedgeoffear/
Soundcloud - https://soundcloud.com/user-290758847

## 📄 License

This project is open-source and licensed under the **GNU General Public License v3.0 (GPLv3)**. See the [LICENSE](LICENSE) file for details.

---

## 💀 Credits

Designed, circuit-modeled, and developed by **The Edge of Fear**.
