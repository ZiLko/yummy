#pragma once

#include "Includes.hpp"

#include <Geode/modify/PlayerObject.hpp>

class $modify(ProPlayerObject, PlayerObject) {
    
    struct Fields {
        bool m_wasOnGround =  false;
        float m_fallingFrom = 0.f;
        float m_prevYVelocity = 0.f;
        int m_shouldDestroy = 0;
    };
    
    void didHitHead();
    
    void updateJump(float);
    
    void incrementJumps();
    
    void togglePlayerScale(bool, bool);
    
    void collidedWithObject(float, GameObject*, CCRect, bool);

    void togglePlayerScale(bool toMini, bool p1);
};