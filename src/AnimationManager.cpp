#include "AnimationManager.hpp"
#include "GJBaseGameLayer.hpp"
#include <random>

AnimationManager& AnimationManager::get() {
    static AnimationManager instance;
    return instance;
}

void AnimationManager::tryStartAbsorbing(GameObject* obj) {
    bool wasFloating = false;
    
    if (m_objectStates.contains(obj) || obj->m_isDisabled || obj->m_isDisabled2) {
        if (m_objectStates.contains(obj)) {
            if (m_objectStates.at(obj).animation == ObjectAnimation::Absorb)
                return;
            else
                wasFloating = true;
        } else
            return;
    }
    
    m_objectStates[obj] = {
        .animation = ObjectAnimation::Absorb,
        .startPosition = obj->getPosition(),
        .startRotation = obj->getRotation(),
        .targetRotation = getRandomRotation()
    };
    
    if (!wasFloating)
        m_animatingObjects.push_back(obj);
}


void AnimationManager::stopAbsorbingAll(const CCPoint& pos) {
    for (GameObject* obj : m_animatingObjects)
        stopAbsorbing(obj, pos);
}

void AnimationManager::stopAbsorbing(GameObject* obj, const CCPoint& pos) {
    ObjectState& state = m_objectStates.at(obj);
    
    if (state.animation != ObjectAnimation::Absorb) return;
    
    state.animation = ObjectAnimation::Float;
    state.time = 0.f;
    state.startRotation = obj->getRotation();
    state.startPosition = obj->getPosition();
    state.floatTo = pos;
}

void AnimationManager::updateAnimations(float dt, ProGJBaseGameLayer* bgl) {
    std::vector<GameObject*> toRemove;
    std::unordered_set<GameObject*> animated;
    // log::debug("{}", m_animatingObjects.size());
    
    for (GameObject* obj : m_animatingObjects) {
        if (!m_objectStates.contains(obj) || animated.contains(obj)) continue;
        
        ObjectState& state = m_objectStates.at(obj);
        bool isFloating = state.animation == ObjectAnimation::Float;
        
        if (isFloating) continue;
        
        animated.insert(obj);
        
        auto f = bgl->m_fields.self();
        
        state.time += dt;
        
        float rot = (state.targetRotation - state.startRotation) * state.time / 2.f;
        obj->setRotation(state.startRotation + rot);
        
        CCPoint targetPos = ccpLerp(obj->getPosition(), bgl->m_player1->getPosition(), state.time / 25.f);
        
        CCPoint delta = targetPos - obj->getPosition();
        CCArray* moveArray = CCArray::create(obj, nullptr);
        bgl->moveObjects(moveArray, delta.x, delta.y, false);
        
        // log::debug("{}", progress);
        
        if (ccpDistance(bgl->m_player1->getPosition(), obj->getPosition()) < 20 * f->m_scaleMultiplier) {
            if (!isFloating) {
                obj->m_isDisabled  = true;
                obj->m_isDisabled2 = true;            
                f->m_scaleMultiplier += GROW_RATE;
            }
            
            toRemove.push_back(obj);
        }
    }
    
    for (GameObject* obj : toRemove) {
        m_objectStates.erase(obj);
        
        auto it = std::find(m_animatingObjects.begin(), m_animatingObjects.end(), obj);
        
        if (it != m_animatingObjects.end())
            m_animatingObjects.erase(it);
    }
}

float AnimationManager::getRandomRotation() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> dist(-ROTATION_VARIATION, ROTATION_VARIATION);
    
    return dist(gen);
}

void AnimationManager::destroyObject(GameObject* obj, int pieces, CCNode* objectLayer, DestroyAnimation animation) {
    if (!obj) return;
    
    if (animation == DestroyAnimation::Break || animation == DestroyAnimation::Pop) {
        CCSize size = obj->getContentSize();
        CCRenderTexture* texture = CCRenderTexture::create(size.width, size.height);
        
        texture->begin();
        
        CCPoint og = obj->getPosition();
        
        obj->setFlipX(false);
        obj->setFlipY(false);
        obj->setRotation(0);
        obj->setPosition(-obj->getPosition() + objectLayer->convertToNodeSpace({0, 0}) + size + objectLayer->getPosition());
        obj->visit();
        obj->setPosition(og);
        
        texture->end();
        
        ExplodeItemNode* explosion = ExplodeItemNode::create(texture);
        explosion->setPosition(obj->getPosition());
        objectLayer->addChild(explosion, 101);
        
        explosion->createSprites(1, pieces, 2.5, 5, 6.f, 3.f, 1.4f, 0.f, {1.f, 1.f, 1.f, 1.f}, {1.f, 1.f, 1.f, 1.f}, false);
    }
    
    obj->m_isDisabled = true;
    obj->m_isDisabled2 = true;
}


void AnimationManager::playResourcesSound(const std::string& soundStr) {
    auto system = FMODAudioEngine::sharedEngine()->m_system;
    log::debug("d");
        
    FMOD::Sound* sound;
    FMOD::Channel* c;

    system->createSound((Mod::get()->getResourcesDir() / soundStr).string().c_str(), FMOD_DEFAULT, nullptr, &sound);
    system->playSound(sound, nullptr, false, &c);
    c->setVolume(Vars::getVolume() / 100.f);
}