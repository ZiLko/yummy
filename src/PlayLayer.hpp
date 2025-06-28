#pragma once

#include "Includes.hpp"

#include <Geode/modify/PlayLayer.hpp>

class $modify(ProPlayLayer, PlayLayer) {

    void setupHasCompleted();

    void addObject(GameObject*);
    
    void destroyPlayer(PlayerObject*, GameObject*);
    
    void onConsume(CCObject*);
};