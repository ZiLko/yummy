#pragma once

#include "Includes.hpp"

#include <Geode/modify/PlayLayer.hpp>

class $modify(ProPlayLayer, PlayLayer) {
    #ifndef GEODE_IS_IOS
    void setupHasCompleted();
    #endif
    void addObject(GameObject*);
    
    void destroyPlayer(PlayerObject*, GameObject*);
    
};