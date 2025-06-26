#include "PlayLayer.hpp"
#include "PlayerObject.hpp"
#include "GJBaseGameLayer.hpp"
#include "AnimationManager.hpp"

#ifndef GEODE_IS_IOS
#include <geode.custom-keybinds/include/Keybinds.hpp>
    
void ProPlayLayer::setupHasCompleted() {
    PlayLayer::setupHasCompleted();
    
    using namespace keybinds;
    
    this->template addEventListener<InvokeBindFilter>([this](InvokeBindEvent* event) {
        ProGJBaseGameLayer* bgl = static_cast<ProGJBaseGameLayer*>(m_player1->m_gameLayer);
        
        event->isDown() ? bgl->startEating() : bgl->stopEating();
        
        return ListenerResult::Propagate;
    }, "backflip"_spr);
}
#endif

void ProPlayLayer::addObject(GameObject* p0) {
    PlayLayer::addObject(p0);
    
    if (p0->m_objectType == GameObjectType::Decoration)
        static_cast<ProGJBaseGameLayer*>(m_player1->m_gameLayer)->m_fields->m_allDecoObjects.push_back(p0);
}

void ProPlayLayer::destroyPlayer(PlayerObject* player, GameObject* obj) {
    if ((player != m_player1 && player != m_player2) || obj == m_anticheatSpike)
        return PlayLayer::destroyPlayer(player, obj);
        
        if (player->m_isOnGround2) {
            player->m_isOnGround = false;
            player->m_isOnGround2 = false;
            player->m_isOnGround3 = false;
            player->m_isOnGround4 = false;
            player->m_yVelocity = 1.f;
        }
        
        if (obj) {
            AnimationManager::destroyObject(obj, 1, m_objectLayer, DestroyAnimation::Pop);
            AnimationManager::playSound("boop.mp3");
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