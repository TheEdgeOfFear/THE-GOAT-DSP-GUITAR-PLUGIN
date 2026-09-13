# User Manual & Instruction Guide — THE GOAT ⚡🐐
### *By THE EDGE OF FEAR*

![THE GOAT Main Interface](Assets/THE%20GOAT.png)

Welcome to the official user manual for **THE GOAT**, the ultimate Swedish Death Metal & Modern Deathcore HM-2 chainsaw distortion plugin. This manual explains every knob, switch, feature, and preset in detail, along with pro tips for dialing in devastating guitar and bass tones.

---

## 🎛️ Control Panel Reference

### 1. Main Chainsaw Stompbox Controls (Center Faceplate)

- **HORNS (Gain / Drive)**:
  - Macro drive knob controlling the input saturation into the non-linear diode clipping engine.
  - Scales from transparent edge-of-breakup boost ($0\text{ dB}$) at minimum, through punchy crunch around 12 o'clock, up to $+48\text{ dB}$ of saturated, singing chainsaw fuzz and infinite sustain when cranked.
- **LOW (100Hz Gyrator)**:
  - Active 2nd-order peaking gyrator filter tuned to $100\text{ Hz}$ ($Q = 2.1$).
  - Range: $-18.0\text{ dB}$ to $+18.0\text{ dB}$.
  - *Pro Tip*: For the authentic Swedish chainsaw tone, set this knob to $+18\text{ dB}$ ("Dime"). To tighten up extreme 8-string drop-tunings (Drop E / Double Drop D), back it down slightly to $+12\text{ dB}$ to $+15\text{ dB}$.
- **HIGH (1.3kHz Buzzsaw Core)**:
  - Active 2nd-order high-$Q$ peaking gyrator filter hard-tuned to $1.3\text{ kHz}$ ($Q = 3.5$).
  - Range: $-20.0\text{ dB}$ to $+20.0\text{ dB}$.
  - *The Chainsaw Signature*: Maxing this control to $+20\text{ dB}$ unleashes the legendary Swedish Death Metal "buzzsaw" resonant screech made famous by Sunlight Studios, Entombed, Dismember, and Bloodbath.
- **LEVEL (Master Output)**:
  - Clean master volume gain control from $-30.0\text{ dB}$ to $+12.0\text{ dB}$.
  - Use this to level-match between bypassed and active states, or push the front-end of subsequent amp simulators and IR loaders harder.

---

### 2. Diode Clipping Engine Switch

Located on the lower left of the faceplate:
- **VINTAGE HM-2 (Mode 0)**:
  - Authentic recreation of the original Japanese Boss HM-2 diode network featuring asymmetric Silicon and Germanium diode pairs.
  - Characteristics: Heavy soft-knee compression, razor-sharp abrasive buzzsaw grind, smooth vintage fuzz saturation, and iconic Swedish death metal flavor.
- **GOAT HOT-ROD (Mode 1)**:
  - Evolution based on Melbourne boutique Kink Pedals "Oath of the Goat" circuitry featuring asymmetric Red LED and Silicon clipping.
  - Characteristics: Extended headroom, explosive dynamic attack, tighter low-end clarity, punchy transients, and aggressive upper-mid bite tailored for modern Deathcore, Slam, and Djent breakdowns.

---

### 3. Stomp Bypass Footswitch & LED Jewel

- **STOMP BYPASS Footswitch**:
  - Heavy-duty 3D stainless steel stomp footswitch for toggling between Active processing and True Bypass.
  - Features an exponential crossfading bypass engine that completely prevents audio clicks or pops when toggling mid-riff.
- **Illuminated Blood-Red LED Jewel**:
  - Illuminates with high-intensity laser glow when the pedal is active, and dims to dark obsidian glass when bypassed.

---

- **INPUT CH (Stereo Channel Routing)**:
  - Selects how audio interface input channels are routed into the DSP engine:
    - `AUTO (Detect)`: Smart auto-sensing. If guitar signal is detected on Left only (Input 1), it automatically duplicates the signal to BOTH outputs (L+R). If on Right only (Input 2), it duplicates to BOTH outputs (L+R). If both channels receive signal, it processes true Stereo.
    - `L (In 1 -> Both)`: Forces Channel 0 (Input 1 / Left) to be processed and outputted to **BOTH Left and Right channels** (both ears in headphones/monitors).
    - `R (In 2 -> Both)`: Forces Channel 1 (Input 2 / Right) to be processed and outputted to **BOTH Left and Right channels**.
    - `STEREO (L+R)`: True independent stereo processing for stereo guitar pre-amps or dual pickup feeds.
    - `MONO SUM`: Sums $(L+R) \times 0.5$ and outputs across both stereo channels.
- **INPUT (-24 dB to +12 dB)**:
  - Input staging trim slider. Allows boosting weak passive single-coil pickups or padding hot active humbuckers (Fishman Fluence, EMG 81) before hitting the pre-emphasis filters.
- **BLEND (0% to 100% Wet)**:
  - Parallel Dry/Wet clean mix knob.
  - Allows blending unclipped clean low-end fundamentals back into the distorted buzzsaw tone. Crucial for bass guitarists and technical metal rhythm tracks.
- **OUTPUT (-30 dB to +12 dB)**:
  - Master volume trim slider mirror.
- **OVERSAMPLE (1x, 2x, 4x, 8x)**:
  - Selects the polyphase IIR oversampling rate for the non-linear distortion engine.
  - `1x (Off)`: Zero latency, minimal CPU.
  - `2x Poly`: Low CPU, fast anti-aliasing.
  - `4x Poly` *(Default)*: Studio mastering-grade anti-aliasing suppressing alias mirror reflections below $-80\text{ dB}$.
  - `8x Poly`: Maximum extreme oversampling for high sample-rate rendering.
- **MIDI MAP**:
  - Opens the interactive MIDI Mapping modal for binding knobs and footswitches to hardware controllers.
- **POWER ON / BYPASS**:
  - Master plugin power button linked to the stomp footswitch.

---

## 💾 Preset System & User Preset Management

**THE GOAT** includes 22+ meticulously dialed-in factory presets categorized by subgenre:

1. **Swedish Death Metal**:
   - *Left Hand Path*: The quintessential Stockholm 1990 death metal tone. Everything dimed.
   - *Like An Everflowing Stream*: Crushing Sunlight Studio buzzsaw tuned for razor-sharp pick attack and 100 Hz rumble.
   - *Bloodbath Chainsaw*: Modern Swedish revival tone with pinned 1.3 kHz resonance.
   - *Clandestine Grind*: Slightly backed-off horns for intricate chord separation.
   - *Skogsberg Sunlight 1989*: Desk routing recreation into a Marshall front end.
2. **Deathcore & Modern Metal**:
   - *Slaughter To Prevail Wall*: Goat Hot-Rod LED clipping mode engaged. Maximum breakdown punch.
   - *Lorna Guttural Buzz*: Aggressive peak designed to sit in front of digital amp profilers.
   - *Fit For An Autopsy Girth*: Hot-rod clipping with 80% wet blend for 7/8-string clarity.
   - *Melbourne Cult Goat*: Boutique custom setting with enhanced headroom and harmonic bloom.
   - *Shadow of Intent Slam*: Heavy palm-muted devastation with boosted input drive.
3. **Grindcore & Powerviolence**:
   - *Unsilent Death Nails*: Total sonic warfare. Pinned gain causing violent intermodulation screech.
   - *Rotten Sound Blast*: Fast transient response for 250+ BPM blastbeats.
   - *Nasum Inhale Grind*: Mid-scoop emphasis with roaring 100 Hz sub rumble.
   - *Napalm Scum Terror*: Raw vintage diode compression for classic Birmingham grind.
4. **Doom, Sludge & Drone**:
   - *Funeral Fuzz Altar*: Earth-shaking low gyrator rumble for drop-A down-tuned dirges.
   - *Crowbar Sludge Hammer*: Swampy NOLA sludge tone with thick low-mid weight.
   - *Electric Wizard Black Mass*: Saturated fuzz boundary where gyrators transform into an occult wail.
   - *Sunn O Drone Monolith*: Endless harmonic sustain for seismic drone chords.
5. **Bass Chainsaw & Clean Blend**:
   - *Dark Tranquillity Bass Saw*: 60% wet parallel clean bass mix preserving low fundamentals.
   - *Djent Parallel Clank*: 50/50 Dry/Wet mix designed to feed a clanky grit channel.
   - *Bloodbath Bass Bulldozer*: Full wet bass destruction with pinned 100 Hz boost.
   - *Modern Edge Booster*: Mild drive used as a clean-blend preamp booster.

### Creating & Saving Custom Presets
1. Adjust the controls to your desired tone.
2. Click the **SAVE** button on the top bar.
3. Type your custom preset name and press **Enter** (or click **SAVE**).
4. Your preset is permanently saved to your user XML bank (`AppData/Roaming/TheEdgeOfFear/TheGoat/UserPresets.xml`) and will appear under the **User Presets** category.
5. To delete a user preset, select it and click **DEL**.

---

## 🎚️ MIDI Learn & Hardware Controller Mapping

You can control every parameter of **THE GOAT** using hardware MIDI controllers, expression pedals, or foot switches:

1. Click the **MIDI MAP** button in the header bar.
2. Click **+ ADD MAPPING** to create a new controller binding.
3. Select the **Target Parameter** (e.g., `HORNS`, `LOW (100Hz)`, `HIGH (1.3kHz)`, `LEVEL`, `BLEND`, `CLIPPING ENGINE`, `STOMP BYPASS`).
4. Select the **Mapping Type**:
   - `CC Absolute`: Continuous knob or expression pedal sweep (0–127 mapped smoothly).
   - `CC Toggle`: Tap MIDI CC switch to toggle on/off or change clipping modes.
   - `CC Gate (Momentary)`: Momentary footswitch (active while held down, bypassed when released).
   - `Program Change`: Send MIDI Program Change messages to switch presets instantly during live performance.
5. Set the **Channel** (Omni or 1–16) and **CC # / Note #**.
6. Close the modal and rock out!
