#pragma once

using namespace geode::prelude;

const float ROTATION_VARIATION = 75.f;
const float GROW_RATE = 0.001f;

enum ObjectAnimation {
    Absorb = 1,
    Float = 2
};

enum DestroyAnimation {
    Break = 1,
    Pop = 2,
    Poof = 3,
    Flush = 4
};

struct ObjectState {
    ObjectAnimation animation = ObjectAnimation::Absorb;
    float time = 0.f;
    CCPoint startPosition = ccp(0, 0);
    float startRotation = 0.f;
    float targetRotation = 0.f;
    bool wasEaten = false;
    CCPoint floatTo = ccp(0, 0);
};

float volumeSetting = 100.f;