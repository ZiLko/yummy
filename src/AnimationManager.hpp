#pragma once

#include "Includes.hpp"

class ProGJBaseGameLayer;

class AnimationManager {

public:

    std::vector<GameObject*> m_animatingObjects;
    
    std::unordered_map<GameObject*, ObjectState> m_objectStates;

    static AnimationManager& get();
    
    static void destroyObject(GameObject*, int, CCNode*, DestroyAnimation);
    
    static void playResourcesSound(const std::string&);

    void tryStartAbsorbing(GameObject*);
    
    void stopAbsorbingAll(const CCPoint&);
    
    void stopAbsorbing(GameObject*, const CCPoint&);
    
    void updateAnimations(float, ProGJBaseGameLayer*);

    void tryAbsorbToPlayer(ProGJBaseGameLayer* bgl, PlayerObject* player, GameObject* obj, ObjectState& state, bool& isFloating, float& scaleMult1, float& scaleMult2, std::vector<GameObject*>& toRemove);

    float getRandomRotation();
    
};