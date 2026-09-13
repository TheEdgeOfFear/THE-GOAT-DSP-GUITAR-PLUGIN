# THE GOAT — Technical Description & DSP Architecture ⚡🐐
### *By THE EDGE OF FEAR*

![THE GOAT Main Interface](Assets/THE%20GOAT.png)

---

## 1. Executive Summary & Circuit Pedigree

**THE GOAT** by **THE EDGE OF FEAR** is a boutique digital emulation of the cult Japanese 1980s **Boss HM-2 Heavy Metal** circuit topology and its Australian boutique evolution, the Melbourne-crafted **Kink Pedals "Oath of the Goat"**.

Unlike conventional overdrive, distortion, or fuzz pedals that rely on basic symmetric diode clipping, the HM-2 "Chainsaw" sound is defined by four interdependent analog circuit stages:
1. **Aggressive Pre-Emphasis Filtering**: Extreme high-pass low-cut and discrete buffer treble boost to focus midrange and treble energy before distortion.
2. **Multi-Stage Asymmetric Diode Saturation**: Dual-polarity asymmetric clipping driving into high-order compression.
3. **Passive RC Mid-Cut Notch**: A fixed mid-scoop around $530\text{ Hz}$ carving out nasal mud prior to the active gyrator filter section.
4. **Active Dual-Gyrator Tone Stack ("The Chainsaw Engine")**: Two high-$Q$, highly resonant active peaking gyrator filters tuned to $100\text{ Hz}$ (Low) and $1.3\text{ kHz}$ (High/Mids). When these controls are maxed ("dimed" to 10), they deliver a ferocious $+18\text{ dB}$ to $+20\text{ dB}$ resonant boost that forms the legendary Swedish Death Metal chainsaw tone.

---

## 2. DSP Block Diagram

```
[Dry Audio Input]
       │
       ▼
[Stage 1: Pre-Emphasis & Discrete Input Buffer Simulation]
  ├── 20.0 Hz DC-Blocker
  ├── 72.3 Hz 2nd-Order Butterworth High-Pass Filter (Low-Cut)
  └── 1.8 kHz High-Shelf Treble Boost (+2.0 dB, Q = 0.707)
       │
       ▼
[Stage 2: Non-Linear Distortion Engine with 4x Polyphase Oversampling]
  ├── 4x Polyphase Half-Band IIR Oversampling (Anti-Aliasing)
  ├── Hybrid Asymmetric Diode Transfer Function
  └── Dual Diode Clipping Engine:
       ├── Mode 0: "Vintage HM-2" (Silicon + Germanium Asymmetric Diode Pair)
       └── Mode 1: "Goat Hot-Rod" (Silicon + Red LED Asymmetric Diode Pair)
       │
       ▼
[Stage 3: Passive Fixed Mid-Scoop Filter]
  └── 530 Hz Notch Filter (Q = 0.8, -6.5 dB attenuation)
       │
       ▼
[Stage 4: Active Dual-Gyrator Tone Stack ("The Chainsaw Core")]
  ├── Low Band Gyrator: 100 Hz Peaking Bell (Q = 2.1, -18 dB to +18 dB, Default: +18 dB)
  └── High Band Gyrator: 1.3 kHz High-Q Peaking Bell (Q = 3.5, -20 dB to +20 dB, Default: +20 dB)
       │
       ▼
[Stage 5: Post-EQ Analog Output Buffer & Cab Roll-Off]
  └── 6.8 kHz 2-Pole S-Plane Butterworth Low-Pass Filter
       │
       ▼
[Stage 6: Output Master Level, Parallel Clean Blend & True-Bypass]
  ├── Input Gain Staging (-24 dB to +12 dB)
  ├── Output Master Level (-30 dB to +12 dB)
  ├── Parallel Dry/Wet Clean Blend (0% to 100% Wet)
  └── Clickless True-Bypass Crossfading
       │
       ▼
[Processed Audio Output]
```

---

## 3. Detailed DSP Module Specifications

### Module 1: Pre-Emphasis & Buffering
- **DC Blocker**: High-pass filter tuned to $20\text{ Hz}$ removing DC offsets.
- **Low-Cut HPF**: 2nd-order Butterworth filter at $72.3\text{ Hz}$ ($Q = 0.7071$). Prevents extreme sub-bass transients from causing intermodulation distortion and mud in high-gain stages.
- **Pre-Tilt High-Shelf**: $+2.0\text{ dB}$ high-shelf boost starting around $1.8\text{ kHz}$ simulating the discrete transistor input buffer loading of vintage boutique stompboxes.

### Module 2: Non-Linear Distortion Engine & 4x Polyphase Oversampling
- **Anti-Aliasing Scaffolding**: Powered by `juce::dsp::Oversampling<float>` running half-band polyphase IIR filters. Operates at $4\times$ (user switchable between 1x, 2x, 4x, and 8x) to ensure alias products and harmonic foldover remain suppressed below $-80\text{ dB}$.
- **Mathematical Diode Transfer Function**:
  $$f(x) = \text{sign}(x) \cdot \left(1.0 - e^{-\alpha \vert{}x\vert{}}\right) + \beta \cdot \tanh(\gamma x)$$
- **Clipping Modes**:
  1. **Vintage HM-2 (Mode 0)**:
     - Emulates back-to-back Silicon ($V_f \approx 0.65\text{V}$) and Germanium ($V_f \approx 0.3\text{V}$) asymmetric diode pairs.
     - Early soft-knee compression on the positive half-wave transitioning into hard rail clamping on the negative half-wave.
     - Produces rich even- and odd-harmonic saturation and the authentic raw Swedish chainsaw grind.
  2. **Goat Hot-Rod (Mode 1)**:
     - Emulates boutique Red LED ($V_f \approx 1.8\text{V} - 2.1\text{V}$) and Silicon diode networks.
     - Higher headroom, explosive transient punch, open dynamic response, and harmonically saturated upper-mid bite tailored for modern Deathcore 7/8-string breakdowns.
- **Drive / Horns Mapping**: Macro taper scaling smoothly from clean boost ($0\text{ dB}$) up to $+48\text{ dB}$ blistering saturated fuzz/distortion.

### Module 3: Passive Fixed Mid-Scoop Filter
- Fixed notch filter centred at $530\text{ Hz}$ ($Q = 0.8$, $-6.5\text{ dB}$).
- Faithfully replicates the passive resistor-capacitor bridge network situated between the distortion op-amp stage and the active tone control, eliminating nasal "honk" and creating space for the gyrator resonance peaks.

### Module 4: Active Dual-Gyrator Tone Stack ("The Chainsaw Engine")
- **Low Frequency Gyrator**:
  - Filter Type: 2nd-order parametric peaking EQ.
  - Centre Frequency: $100\text{ Hz}$ ($f_0$).
  - Bandwidth / $Q$: $Q \approx 2.1$.
  - Range: $-18\text{ dB}$ to $+18\text{ dB}$ (Default $+18\text{ dB}$ Dime).
- **High Frequency Gyrator (The Buzzsaw Core)**:
  - Filter Type: 2nd-order high-$Q$ peaking bell EQ.
  - Centre Frequency: $1.3\text{ kHz}$ ($f_0$).
  - Bandwidth / $Q$: $Q \approx 3.5$ (Extreme resonance peak).
  - Range: $-20\text{ dB}$ to $+20\text{ dB}$ (Default $+20\text{ dB}$ Dime).
- **Zipper-Free Parameter Smoothing**: Gyrator filter gains are interpolated per sample using `juce::SmoothedValue` with linear smoothing times to ensure click-free knob sweeps during live automation and MIDI control.

### Module 5: Post-EQ Cab Filtering & Output Stage
- **Output Low-Pass Roll-Off**: 2-pole Butterworth low-pass filter at $6.8\text{ kHz}$ to roll off piercing ultrasonic buzz, simulating the analog output stage and guitar cabinet speaker roll-off.
- **Master Volume**: Clean output gain control from $-30\text{ dB}$ to $+12\text{ dB}$.
- **Parallel Clean Blend**: Clean dry / processed wet mix knob ($0\%$ to $100\%$ wet). Essential for modern metal bassists and 8-string players who require pristine low-end note clarity blended with the brutal chainsaw top-end.
- **Click-Free True Bypass**: Exponential crossfade circuit eliminating pops and clicks during footswitch engagement.

---

## 4. Parameter Reference Table

| Parameter ID | Parameter Name | Control Type | Range | Default | Description |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `dist` | **HORNS** | Rotary Knob | `0.0 – 1.0` | `0.75` | Drive / Gain intensity ($0\text{ dB}$ to $+48\text{ dB}$). |
| `low` | **LOW (100Hz)** | Rotary Knob | `-18.0 – +18.0 dB` | `+18.0 dB` | Active $100\text{ Hz}$ Gyrator peaking boost/cut. |
| `high` | **HIGH (1.3kHz)** | Rotary Knob | `-20.0 – +20.0 dB` | `+20.0 dB` | Active $1.3\text{ kHz}$ Buzzsaw Gyrator resonance boost/cut. |
| `vol` | **LEVEL** | Rotary Knob | `-30.0 – +12.0 dB` | `0.0 dB` | Master output volume. |
| `blend` | **BLEND** | Rotary Knob | `0.0 – 1.0 (0-100%)` | `1.0 (100% Wet)` | Parallel dry/wet blend path. |
| `inGain` | **INPUT** | Rotary Knob | `-24.0 – +12.0 dB` | `0.0 dB` | Input staging trim level. |
| `mode` | **CLIPPING ENGINE** | 2-Way Toggle | `0 / 1` | `0 (Vintage HM-2)` | Silicon+Germanium vs Silicon+LED diode clipping. |
| `power` | **STOMP BYPASS** | Stomp Switch | `Off / On` | `On (Active)` | Heavy-duty true bypass footswitch with glowing LED. |
| `oversample`| **OVERSAMPLE** | Dropdown Menu | `1x, 2x, 4x, 8x` | `4x Polyphase` | Anti-aliasing oversampling factor. |

---

## 5. System Requirements & Compatibility

- **Operating System**: Windows 10 / 11 (64-bit)
- **Plugin Format**: VST3 (64-bit), Standalone EXE (64-bit), DLL (64-bit)
- **Sample Rates**: $44.1\text{ kHz}$, $48\text{ kHz}$, $88.2\text{ kHz}$, $96\text{ kHz}$, $192\text{ kHz}$
- **Host Compatibility**: Reaper, Cubase, Nuendo, Ableton Live, FL Studio, Studio One, Bitwig, Pro Tools (via VST3 wrapper), Cakewalk.
