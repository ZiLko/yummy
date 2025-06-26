#pragma once

#include "Includes.hpp"

class ProGJBaseGameLayer;

class AnimationManager {
  
public:

    static AnimationManager& get();
    
    static void destroyObject(GameObject*, int, CCNode*, DestroyAnimation);
    
    static void playSound(const std::string&);

    std::vector<GameObject*> m_animatingObjects;
    
    std::unordered_map<GameObject*, ObjectState> m_objectStates;

    void tryStartAbsorbing(GameObject*);
    
    void stopAbsorbingAll(const CCPoint&);
    
    void stopAbsorbing(GameObject*, const CCPoint&);
    
    void updateAnimations(float, ProGJBaseGameLayer*);
    
    float getRandomRotation();
    
};