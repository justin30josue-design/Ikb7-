#include "buffer_engine.hpp"
#include <Geode/binding/GJBaseGameLayer.hpp>

using namespace geode::prelude;

void InputBufferEngine::pushEvent(bool isPush, int button, bool isPlayer2) {
    if (m_bufferFrames == 0) {
        // Direct dispatch for 0-buffer setting
        if (auto gameLayer = GJBaseGameLayer::get()) {
            gameLayer->handleButton(isPush, static_cast<PlayerButton>(button), isPlayer2);
        }
        return;
    }

    BufferedInputEvent evt{
        .isPush = isPush,
        .button = button,
        .isPlayer2 = isPlayer2,
        .scheduledTick = m_currentTick + static_cast<uint64_t>(m_bufferFrames)
    };
    m_eventQueue.push_back(evt);
}

void InputBufferEngine::onPhysicsTick(GJBaseGameLayer* gameLayer) {
    m_currentTick++;

    if (!gameLayer || m_eventQueue.empty()) return;

    while (!m_eventQueue.empty()) {
        auto& frontEvt = m_eventQueue.front();
        if (frontEvt.scheduledTick <= m_currentTick) {
            gameLayer->handleButton(
                frontEvt.isPush,
                static_cast<PlayerButton>(frontEvt.button),
                frontEvt.isPlayer2
            );
            m_eventQueue.pop_front();
        } else {
            break; // Queue is tick-sorted
        }
    }
}

void InputBufferEngine::clear() {
    m_eventQueue.clear();
    m_currentTick = 0;
}