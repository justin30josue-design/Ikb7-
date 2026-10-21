#include <Geode/Geode.hpp>
#include "buffer_engine.hpp"

using namespace geode::prelude;

$execute {
    // Sync settings on startup and settings modification
    auto updateSettings = +[]() {
        int buffer = static_cast<int>(Mod::get()->getSettingValue<int64_t>("buffer-frames"));
        InputBufferEngine::get().setBufferFrames(buffer);
    };

    updateSettings();

    listenForSettingChanges("buffer-frames", [](int64_t val) {
        InputBufferEngine::get().setBufferFrames(static_cast<int>(val));
    });
}