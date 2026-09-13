# Installation Guide — THE GOAT ⚡🐐
### *By THE EDGE OF FEAR*

![THE GOAT Main Interface](Assets/THE%20GOAT.png)

This guide provides step-by-step instructions for installing and setting up **THE GOAT** on Windows systems across all 3 formats: **VST3 Plugin**, **Standalone Desktop App**, and **DLL**.

---

## 🚀 Option 1: 1-Click Automated VST3 Installer (Recommended)

The easiest and fastest way to install **THE GOAT** for your DAW is using the included batch installer:

1. Download or clone this repository to your local drive.
2. Navigate to the root directory (or `/VST3` folder).
3. Right-click **`INSTALL_THE_GOAT_VST3.bat`** and select **"Run as administrator"**.
4. The installer script will automatically:
   - Clean any previous versions from your system VST3 directory.
   - Deploy `THE GOAT.vst3` bundle directly into `C:\Program Files\Common Files\VST3\THE GOAT.vst3`.
   - Verify file integrity and output a success confirmation.
5. Open or restart your DAW and rescan plugins!

---

## 🛠️ Option 2: Manual VST3 Installation

If you prefer to install the plugin manually:

1. Open File Explorer and navigate to the `/VST3` folder in this repository.
2. Copy the entire **`THE GOAT.vst3`** folder.
3. Paste it into your system's standard 64-bit VST3 directory:
   ```text
   C:\Program Files\Common Files\VST3\
   ```
4. Verify the folder structure looks like this:
   ```text
   C:\Program Files\Common Files\VST3\THE GOAT.vst3\Contents\x86_64-win\THE GOAT.vst3
   C:\Program Files\Common Files\VST3\THE GOAT.vst3\Contents\Resources\moduleinfo.json
   ```
5. Launch your DAW and perform a plugin rescan.

---

## 🎸 Option 3: Standalone Desktop Application

**THE GOAT** includes a native standalone executable that lets you plug in your guitar and play without launching a DAW:

1. Navigate to the `/Standalone` folder in this repository.
2. Run **`THE GOAT.exe`**.
3. Configure your audio interface:
   - Click **Options -> Audio/MIDI Settings** (or the top settings menu).
   - **Audio Device Type**: Select **ASIO** (e.g., Focusrite USB ASIO, ASIO4ALL, Universal Audio ASIO, FL Studio ASIO, FlexASIO).
   - **Sample Rate**: Set to **48,000 Hz** or **44,100 Hz**.
   - **Audio Buffer Size**: Set to **64, 128, or 256 samples** for ultra-low latency playing.
   - **Input Channels**: Select your guitar instrument input channel.
   - **Output Channels**: Select your monitor/headphone output channels.
4. Set up MIDI (Optional):
   - Under **Active MIDI Inputs**, tick your MIDI foot controller (e.g., FCB1010, Line 6 FBV, Morningstar MC6, generic MIDI pedal).

---

## 🔌 Option 4: DLL Installation

For legacy hosts, modular environments, or plugin host wrappers requiring a direct 64-bit DLL:

1. Navigate to the `/DLL` folder.
2. Copy **`THE GOAT.dll`**.
3. Paste into your custom 64-bit VST directory (e.g., `C:\Program Files\VstPlugins\` or `C:\Program Files\Steinberg\VstPlugins\`).

---

## 🎛️ DAW-Specific Setup Guides

### Cockos REAPER
1. Go to **Options -> Preferences -> Plug-ins -> VST**.
2. Ensure `C:\Program Files\Common Files\VST3` is in your VST search paths.
3. Click **Re-scan -> Clear cache and re-scan VST paths**.
4. Insert a new track, click **FX**, and search for `"THE GOAT"` (under **VST3: THE GOAT (THE EDGE OF FEAR)**).

### Steinberg Cubase / Nuendo
1. Go to **Studio -> VST Plugin Manager**.
2. Click the **Rescan All** button (circle icon).
3. Insert **THE GOAT** on any Audio or Instrument track insert slot.

### Ableton Live
1. Go to **Preferences -> Plug-Ins**.
2. Ensure **"Use VST3 Plug-In System Folders"** is turned **ON**.
3. Hold `Alt` and click **Rescan** to force a fresh plugin scan.
4. In the browser under **Plug-Ins -> VST3 -> THE EDGE OF FEAR**, drag **THE GOAT** onto your channel strip.

### Image-Line FL Studio
1. Go to **Options -> Manage Plugins**.
2. Ensure `C:\Program Files\Common Files\VST3` is checked in plugin search paths.
3. Enable **"Rescan previously verified plugins"** and click **Find installed plugins**.
4. Add **THE GOAT** from the Effects list onto your Mixer insert.

### PreSonus Studio One
1. Go to **Studio One -> Options -> Locations -> VST Plug-Ins**.
2. Click **Reset Blacklist** and **Scan at startup**.
3. Find **THE GOAT** under the **Effects -> Vendor -> THE EDGE OF FEAR** browser.

---

## ❓ Troubleshooting & FAQs

- **Plugin not appearing in DAW?**
  Make sure your DAW is 64-bit (x64). Ensure the path `C:\Program Files\Common Files\VST3\THE GOAT.vst3` exists and was not placed in `Program Files (x86)`.
- **High CPU or crackles?**
  Set oversampling to **2x Polyphase** or **4x Polyphase** in the plugin header, and ensure your audio interface buffer size is set to at least 128 samples.
- **Windows SmartScreen warning when running standalone EXE or installer?**
  Click **"More Info" -> "Run anyway"**. The binary is completely safe and compiled directly from the open-source JUCE C++ codebase.
