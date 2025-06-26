#include "PlayerObject.hpp"
#include "GJBaseGameLayer.hpp"
#include "AnimationManager.hpp"

void ProPlayerObject::didHitHead() {
    PlayerObject::didHitHead();
    
    if (!m_gameLayer) return;
    
    ProGJBaseGameLayer* bgl = static_cast<ProGJBaseGameLayer*>(m_gameLayer);
    
    bgl->m_fields->m_checkCollisions = 4;
    bgl->m_fields->m_collisionDirection = PlayerCollisionDirection::Top;
}

void ProPlayerObject::updateJump(float dt) {
    if (!m_gameLayer) return PlayerObject::updateJump(dt);
    
    auto f = m_fields.self();
    auto f2 = static_cast<ProGJBaseGameLayer*>(m_gameLayer)->m_fields.self();
    float scale = f2->m_scaleMultiplier;
    
    m_snapDistance = 0.f;
    
    if (scale > 1.f)
        m_vehicleSize = 1.f;
        
    PlayerObject::updateJump(dt);
    
    if (scale > 1.f)
        m_vehicleSize = scale;
        
    float feet = (getPositionY() - 20 * scale);
    
    if (f->m_prevYVelocity >= 0 && m_yVelocity < 0)
        f->m_fallingFrom = feet;
        
    if (f->m_wasOnGround != m_isOnGround2 && m_isOnGround2 && scale > 1.5f) {
        m_gameLayer->shakeCamera(scale * 0.09f, scale * 0.6f, 0);
        
        if (f->m_fallingFrom > 200 && scale > 2.f) {
            f2->m_checkCollisions = 4;
            f2->m_collisionDirection = PlayerCollisionDirection::Bottom;
        }
        
        f->m_fallingFrom = feet;
        
        // play effect
    }
        
    f->m_wasOnGround = m_isOnGround2;
    f->m_prevYVelocity = m_yVelocity;
}

void ProPlayerObject::incrementJumps() {
    PlayerObject::incrementJumps();
    
    if (!m_gameLayer) return;
    
    ProGJBaseGameLayer* bgl = static_cast<ProGJBaseGameLayer*>(m_gameLayer);
    float scale = bgl->m_fields->m_scaleMultiplier;
    
    if (scale > 1.f)
        m_yVelocity *= (scale - 1) * 0.15f + 1;
}

void ProPlayerObject::togglePlayerScale(bool p0, bool p1) {
    if (!m_gameLayer) return PlayerObject::togglePlayerScale(p0, p1);
    // ProGJBaseGameLayer* bgl = static_cast<ProGJBaseGameLayer*>(m_gameLayer);
    
    // if (bgl->m_fields->m_scaleMultiplier <= 1.f)
        // PlayerObject::
}
void ProPlayerObject::collidedWithObject(float p0, GameObject* p1, CCRect p2, bool p3) {
    PlayerObject::collidedWithObject(p0, p1, p2, p3);
    
    if (!p1 || !m_gameLayer) return;
    
    auto f = m_fields.self();
    
    if (f->m_shouldDestroy > 0 && p1->m_objectType == GameObjectType::Solid) {
        f->m_shouldDestroy--;
        
        AnimationManager::destroyObject(p1, 3, m_gameLayer->m_objectLayer, DestroyAnimation::Break);
    }
}

void ProPlayerObject::togglePlayerScale(bool toMini, bool p1) {
    PlayerObject::togglePlayerScale(toMini, p1);
    if (!this->m_gameLayer) return;
    static_cast<ProGJBaseGameLayer*>(this->m_gameLayer)->reactToPlayerScaleToggle(this, toMini);
}