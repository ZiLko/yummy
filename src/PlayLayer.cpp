#include "PlayLayer.hpp"
#include "PlayerObject.hpp"
#include "GJBaseGameLayer.hpp"
#include "AnimationManager.hpp"

#ifndef GEODE_IS_IOS
#include <geode.custom-keybinds/include/Keybinds.hpp>
#endif

void ProPlayLayer::setupHasCompleted() {
    PlayLayer::setupHasCompleted();
    
    #ifndef GEODE_IS_IOS
    using namespace keybinds;
    
    this->template addEventListener<InvokeBindFilter>([this](InvokeBindEvent* event) {
        ProGJBaseGameLayer* bgl = static_cast<ProGJBaseGameLayer*>(m_player1->m_gameLayer);
        auto f = bgl->m_fields.self();
        event->isDown() ? bgl->startEating() : bgl->stopEating();
        return ListenerResult::Propagate;
    }, "consume-decoration"_spr);
    #endif

    // #ifdef GEODE_IS_MOBILE
    // button for mobile
    if (!m_uiLayer || !m_uiLayer->m_pauseBtn || !m_uiLayer->m_pauseBtn->getParent()) return log::info("could not find UILayer or its pause button :(");
    CCSprite* consumeSprite = CCSprite::create("consume.png"_spr);
    if (!consumeSprite || consumeSprite->getUserObject("geode.texture-loader/fallback")) return log::info("could not create sprite consume.png_spr");
    CCMenuItemSpriteExtra* consumeButton = CCMenuItemSpriteExtra::create(consumeSprite, this, menu_selector(ProPlayLayer::onConsume));
    consumeButton->setID("consume-button-mobile-only"_spr);
    consumeButton->setTag(6252025);
    m_uiLayer->m_pauseBtn->getParent()->addChild(consumeButton);
    consumeButton->setPosition(m_uiLayer->m_pauseBtn->getPosition());
    consumeButton->setPositionY(consumeButton->getPositionY() - 30.f);
    consumeButton->setOpacity(128);
    // add button for mobile here
    // #endif
}

void ProPlayLayer::addObject(GameObject* p0) {
    PlayLayer::addObject(p0);
    if (p0->m_objectType != GameObjectType::Decoration) return;
    static_cast<ProGJBaseGameLayer*>(m_player1->m_gameLayer)->m_fields->m_allDecoObjects.push_back(p0);
}

void ProPlayLayer::destroyPlayer(PlayerObject* player, GameObject* obj) {
    if ((player != m_player1 && player != m_player2) || obj == m_anticheatSpike) return PlayLayer::destroyPlayer(player, obj);
        
    if (player->m_isOnGround2) {
        player->m_isOnGround = false;
        player->m_isOnGround2 = false;
        player->m_isOnGround3 = false;
        player->m_isOnGround4 = false;
        player->m_yVelocity = 1.f;
    }
    
    if (obj) {
        AnimationManager::destroyObject(obj, 1, m_objectLayer, DestroyAnimation::Pop);
        AnimationManager::playResourcesSound("boop.mp3");
    } else {
        auto f = static_cast<ProGJBaseGameLayer*>(static_cast<GJBaseGameLayer*>(this))->m_fields.self();
        
        static_cast<ProPlayerObject*>(player)->m_fields->m_shouldDestroy += 2;
        
        f->m_checkCollisions = 3;
        
        if (!player->m_isSideways)
            f->m_collisionDirection = player->isFlipX() ? PlayerCollisionDirection::Left : PlayerCollisionDirection::Right;
        else
            f->m_collisionDirection = player->isFlipX() ? PlayerCollisionDirection::Bottom : PlayerCollisionDirection::Top;
    }
}

void ProPlayLayer::onConsume(CCObject* sender) {
    ProGJBaseGameLayer* bgl = static_cast<ProGJBaseGameLayer*>(m_player1->m_gameLayer);
    auto f = bgl->m_fields.self();
    f->m_isEating = !f->m_isEating;
    f->m_isEating ? bgl->startEating() : bgl->stopEating();
}