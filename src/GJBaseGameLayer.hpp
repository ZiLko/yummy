#pragma once

#include "Includes.hpp"

#include <Geode/modify/GJBaseGameLayer.hpp>

class $modify(ProGJBaseGameLayer, GJBaseGameLayer) {
    
    struct Fields {
        std::vector<GameObject*> m_allDecoObjects;
        std::vector<GameObject*> m_destroyObjects;
        std::unordered_set<GameObject*> m_destroyedObjects;
        
        PlayerCollisionDirection m_collisionDirection = PlayerCollisionDirection::Top;
        
        float m_scaleMultiplier = 1.f;
        int m_checkCollisions = 0;
        bool m_isEating = false;
    };
    
    void startEating();
    
    void stopEating();
    
    void processCommands(float);
    
    void processMoveActions();
    
    void collisionCheckObjects(PlayerObject*, gd::vector<GameObject*>*, int, float);
		
};