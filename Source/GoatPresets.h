#pragma once

#include <string>
#include <vector>

namespace GoatPresets
{

struct Preset
{
    std::string category;
    std::string name;
    std::string description;
    float dist = 0.75f;         // 0.0 - 1.0 (Gain / Horns)
    float low = 18.0f;          // -18 to +18 dB (Low Gyrator 100 Hz)
    float high = 20.0f;         // -20 to +20 dB (High Gyrator 1.3 kHz)
    float volume = 0.0f;        // -30 to +12 dB
    float blend = 1.0f;         // 0.0 - 1.0 (100% wet)
    int clippingMode = 0;       // 0 = Vintage HM-2, 1 = Goat Hot-Rod
    float inputGain = 0.0f;     // -24 to +12 dB
    int oversample = 2;         // 0=1x, 1=2x, 2=4x, 3=8x (Default 4x)
    bool isFactory = true;
};

inline std::vector<Preset> getBuiltInPresets()
{
    return {
        // === SWEDISH DEATH METAL (TRUE CHAINSAW) ===
        {
            "Swedish Death Metal",
            "Left Hand Path",
            "The quintessential Stockholm 1990 death metal tone. Everything dimed, silicon/germanium asymmetric saturation tearing through the mix.",
            0.85f, 18.0f, 20.0f, 0.0f, 1.0f, 0, 0.0f, 2, true
        },
        {
            "Swedish Death Metal",
            "Like An Everflowing Stream",
            "Crushing Sunlight Studio buzzsaw tuned for razor-sharp pick attack and thunderous bottom end resonance at 100 Hz.",
            0.90f, 18.0f, 20.0f, 1.5f, 1.0f, 0, 1.0f, 2, true
        },
        {
            "Swedish Death Metal",
            "Bloodbath Chainsaw",
            "Modern Swedish revival tone. Maximum high-Q 1.3 kHz screaming resonance pinned to 10 with tightened low end.",
            0.95f, 16.5f, 20.0f, 0.0f, 1.0f, 0, 0.0f, 2, true
        },
        {
            "Swedish Death Metal",
            "Clandestine Grind",
            "Slightly backed-off horns allowing complex chords to maintain separation while keeping the iconic buzzsaw bite.",
            0.70f, 17.0f, 19.0f, 0.0f, 1.0f, 0, 0.0f, 2, true
        },
        {
            "Swedish Death Metal",
            "Skogsberg Sunlight 1989",
            "Recreation of the original Studio Sunlight desk routing with the classic Boss HM-2 into a Marshall JCM800 front end.",
            0.80f, 18.0f, 18.5f, -1.0f, 1.0f, 0, 0.0f, 2, true
        },

        // === DEATHCORE & MODERN METAL ===
        {
            "Deathcore & Modern Metal",
            "Slaughter To Prevail Wall",
            "Goat Hot-Rod LED clipping mode engaged. Maximum punch, brutal transients, and screaming high mids for ultra-heavy breakdowns.",
            0.85f, 18.0f, 20.0f, 0.5f, 1.0f, 1, 1.5f, 2, true
        },
        {
            "Deathcore & Modern Metal",
            "Lorna Guttural Buzz",
            "Aggressive high-resonance peak combined with tight low punch designed to sit in front of high-gain digital amp profilers.",
            0.75f, 15.0f, 20.0f, 0.0f, 0.95f, 1, 0.0f, 2, true
        },
        {
            "Deathcore & Modern Metal",
            "Fit For An Autopsy Girth",
            "Hot-rod clipping with parallel clean blend at 80% wet, retaining note attack for technical 7 and 8-string riffing.",
            0.68f, 18.0f, 17.5f, 1.0f, 0.80f, 1, 0.0f, 2, true
        },
        {
            "Deathcore & Modern Metal",
            "Melbourne Cult Goat",
            "Boutique Melbourne custom HM-2 setting with enhanced headroom and ferocious harmonic bloom.",
            0.92f, 18.0f, 20.0f, 2.0f, 1.0f, 1, 2.0f, 2, true
        },
        {
            "Deathcore & Modern Metal",
            "Shadow of Intent Slam",
            "Heavy palm-muted devastation with boosted input drive and sculpted 530 Hz mid-cut clarity.",
            0.88f, 17.5f, 19.5f, 0.0f, 1.0f, 1, 1.0f, 2, true
        },

        // === GRINDCORE & POWERVIOLENCE ===
        {
            "Grindcore & Powerviolence",
            "Unsilent Death Nails",
            "Total sonic warfare. Pinned gain and dimed gyrators causing violent intermodulation screech on blasting powerchords.",
            1.00f, 18.0f, 20.0f, 2.5f, 1.0f, 0, 3.0f, 2, true
        },
        {
            "Grindcore & Powerviolence",
            "Rotten Sound Blast",
            "Fast transient response, abrasive high-end slice, and explosive midrange chainsaw buzz for 250+ BPM grind beats.",
            0.82f, 16.0f, 20.0f, 0.5f, 1.0f, 0, 0.0f, 2, true
        },
        {
            "Grindcore & Powerviolence",
            "Nasum Inhale Grind",
            "Mid-scoop emphasis with roaring 100 Hz sub rumble and blisteringly sharp 1.3 kHz cut.",
            0.90f, 18.0f, 19.0f, 0.0f, 1.0f, 0, 0.5f, 2, true
        },
        {
            "Grindcore & Powerviolence",
            "Napalm Scum Terror",
            "Raw vintage diode compression and sputtering square-wave saturation for classic Birmingham grindcore.",
            0.95f, 18.0f, 18.0f, 1.0f, 1.0f, 0, 2.0f, 2, true
        },

        // === DOOM, SLUDGE & DRONE ===
        {
            "Doom & Sludge",
            "Funeral Fuzz Altar",
            "Slow-burning, earth-shaking low gyrator rumble with massive asymmetric clipping sustain for drop-A down-tuned dirges.",
            0.80f, 18.0f, 15.0f, 0.0f, 1.0f, 0, 1.0f, 2, true
        },
        {
            "Doom & Sludge",
            "Crowbar Sludge Hammer",
            "Heavy swampy NOLA sludge tone. Hot-rod clipping mode providing thick, greasy low-mid weight.",
            0.72f, 18.0f, 16.0f, 1.5f, 1.0f, 1, 1.0f, 2, true
        },
        {
            "Doom & Sludge",
            "Electric Wizard Black Mass",
            "Saturated fuzz boundary where the HM-2 gyrator filter resonance transforms into an occult fuzz wail.",
            0.92f, 18.0f, 18.0f, 2.0f, 1.0f, 0, 2.5f, 2, true
        },
        {
            "Doom & Sludge",
            "Sunn O Drone Monolith",
            "Endless harmonic sustain, deep 100 Hz sub resonance, and smooth top-end roll-off for seismic drone chords.",
            0.85f, 18.0f, 14.0f, 0.0f, 1.0f, 0, 0.0f, 2, true
        },

        // === BASS CHAINSAW & CLEAN BLEND ===
        {
            "Bass Chainsaw & Clean Blend",
            "Dark Tranquillity Bass Saw",
            "Parallel clean bass mix (60% wet, 40% dry) preserving low-end fundamentals while adding the Swedish chainsaw top.",
            0.75f, 14.0f, 20.0f, 1.0f, 0.60f, 1, 0.0f, 2, true
        },
        {
            "Bass Chainsaw & Clean Blend",
            "Djent Parallel Clank",
            "50/50 Dry/Wet mix designed to feed a clanky grit channel into modern metal bass processing chains.",
            0.65f, 12.0f, 19.0f, 0.0f, 0.50f, 1, 0.0f, 2, true
        },
        {
            "Bass Chainsaw & Clean Blend",
            "Bloodbath Bass Bulldozer",
            "Full wet bass destruction with pinned 100 Hz low-end boost and screaming 1.3 kHz pick scrape.",
            0.85f, 18.0f, 18.0f, 0.5f, 0.85f, 0, 1.0f, 2, true
        },
        {
            "Bass Chainsaw & Clean Blend",
            "Modern Edge Booster",
            "Mild drive with subtle gyrator shaping used as a clean-blend preamp booster in front of high-gain amplifiers.",
            0.40f, 10.0f, 14.0f, 0.0f, 0.70f, 1, 0.0f, 2, true
        }
    };
}

} // namespace GoatPresets
