#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../buffer_engine.hpp"

using namespace geode::prelude;

class $modify(BufferGameLayer, GJBaseGameLayer) {
    void processCommands() {
        // Hook processCommands to drive buffer tick pipeline
        InputBufferEngine::get().onPhysicsTick(this);
        GJBaseGameLayer::processCommands();
    }

    void resetLevel() {
        InputBufferEngine::get().clear();
        GJBaseGameLayer::resetLevel();
    }

    /* Intercept key inputs and route through buffer engine */
    void handleButton(bool down, PlayerButton button, bool isPlayer2) {
        // If current thread or call comes from our engine dispatch, pass down to native
        static bool isDispatching = false;
        if (isDispatching || InputBufferEngine::get().getBufferFrames() == 0) {
            GJBaseGameLayer::handleButton(down, button, isPlayer2);
            return;
        }

        isDispatching = true;
        InputBufferEngine::get().pushEvent(down, static_cast<int>(button), isPlayer2);
        isDispatching = false;
    }
};

class $modify(BufferPlayLayer, PlayLayer) {
    void onQuit() {
        InputBufferEngine::get().clear();
        PlayLayer::onQuit();
    }

    void destroyPlayer(PlayerObject* player, GameObject* object) {
        InputBufferEngine::get().clear();
        PlayLayer::destroyPlayer(player, object);
    }
};