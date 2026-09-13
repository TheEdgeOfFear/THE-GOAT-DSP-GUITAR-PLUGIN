cmake_minimum_required(VERSION 3.22)

# If built standalone without parent project:
if(NOT TARGET juce::juce_audio_basics)
    project(TheGoat VERSION 1.0.0 LANGUAGES C CXX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    include(FetchContent)
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        8.0.4
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(JUCE)
endif()

juce_add_plugin(TheGoat
    COMPANY_NAME "THE EDGE OF FEAR"
    BUNDLE_ID com.theedgeoffear.thegoat
    PLUGIN_MANUFACTURER_CODE TEOF
    PLUGIN_CODE TGot
    FORMATS VST3 Standalone
    PRODUCT_NAME "THE GOAT"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(TheGoat
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        Source/GoatLookAndFeel.h
        Source/GoatPresets.h
        Source/PresetManager.h
        Source/MidiManager.h
        Source/MidiMappingModal.h
        Source/DSP/GoatDSPChain.h
)

juce_add_binary_data(Goat_Assets
    SOURCES
        "Background Image/THE GOAT BG.jpg"
)

target_link_libraries(TheGoat
    PRIVATE
        Goat_Assets
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(TheGoat
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_ASIO=1
)

target_include_directories(TheGoat
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../asiosdk/common"
        "${CMAKE_CURRENT_SOURCE_DIR}/../asiosdk/asiosdk_2.3.3_2019-06-14/common"
)

if(MSVC)
    target_compile_options(TheGoat PUBLIC /utf-8)
endif()
