#include "GJBaseGameLayer.hpp"
#include "AnimationManager.hpp"

#include <random>

void ProGJBaseGameLayer::startEating() {
    if (LevelEditorLayer::get()) return;
    m_fields->m_isEating = true;
}

void ProGJBaseGameLayer::stopEating() {
    if (LevelEditorLayer::get()) return;
    m_fields->m_isEating = false;
    AnimationManager::get().stopAbsorbingAll(m_player1->getPosition());
    AnimationManager::get().stopAbsorbingAll(m_player2->getPosition());
}

void ProGJBaseGameLayer::processCommands(float dt) {
    GJBaseGameLayer::processCommands(dt);
    if (LevelEditorLayer::get()) return;
    
    AnimationManager::get().updateAnimations(dt, this);
    
    auto f = m_fields.self();
    
    m_player1->m_vehicleSize = f->m_scaleMultiplier;
    m_player1->setScale(f->m_scaleMultiplier);
    m_player1->m_stateScale = f->m_scaleMultiplier;
    m_player1->m_spriteWidthScale = f->m_scaleMultiplier;
    m_player1->m_spriteHeightScale = f->m_scaleMultiplier;
    
    m_player2->m_vehicleSize = f->m_scaleMultiplierPlayerTwo;
    m_player2->setScale(f->m_scaleMultiplierPlayerTwo);
    m_player2->m_stateScale = f->m_scaleMultiplierPlayerTwo;
    m_player2->m_spriteWidthScale = f->m_scaleMultiplierPlayerTwo;
    m_player2->m_spriteHeightScale = f->m_scaleMultiplierPlayerTwo;
    
    if (f->m_scaleMultiplier > 1.f) {
        m_gameState.tweenValue(m_gameState.m_cameraZoom, std::pow(f->m_scaleMultiplier, -0.642f), 14, 0.2f, 1, 1, 0, 0);
        
        // if (zoom < 0.4f) {
        //     m_gameState.m_cameraZoom = zoom;
        //     m_objectLayer->setScale(zoom);
        // } else
        //     updateZoom(zoom, 0.2f, 1, 1, 0, 0);
    }
}

void ProGJBaseGameLayer::processMoveActions() {
    GJBaseGameLayer::processMoveActions();
    if (LevelEditorLayer::get()) return;
    
    //m_effectManager->m_unkVector6d8.size(), m_effectManager->m_unkVector6f0.size()
    auto f = m_fields.self();
    
    if (!f->m_isEating) return;
    
    for (GameObject* obj : f->m_allDecoObjects) {
        if (ccpDistance(m_player1->getPosition(), obj->getPosition()) < 20 * f->m_scaleMultiplier + 125) {
            obj->m_objectType = GameObjectType::Decoration;
            AnimationManager::get().tryStartAbsorbing(obj);
        }
    }

    // for (CCMoveCNode* obj : m_effectManager->m_unkVector6c0) {

    // }
}

void ProGJBaseGameLayer::collisionCheckObjects(PlayerObject* player, gd::vector<GameObject*>* p1, int p2, float p3) {      
    GJBaseGameLayer::collisionCheckObjects(player, p1, p2, p3);
    if (LevelEditorLayer::get() || (player != m_player1)) return;
    
    auto f = m_fields.self();
    
    if (f->m_checkCollisions <= 0) return;
    
    f->m_checkCollisions--;
        
    for (int i = 0; i < p2; i++) {
        GameObject* obj = p1->at(i);
        if (!obj) continue;
        bool valid = false;
        
        if (f->m_collisionDirection == PlayerCollisionDirection::Top)
            valid = obj->getPositionY() > player->getPositionY();
        else if (f->m_collisionDirection == PlayerCollisionDirection::Bottom)
            valid = obj->getPositionY() < player->getPositionY();
        else if (f->m_collisionDirection == PlayerCollisionDirection::Left) 
            valid = obj->getPositionX() < player->getPositionX();
        else if (f->m_collisionDirection == PlayerCollisionDirection::Right)
            valid = obj->getPositionX() > player->getPositionX();
        
        if (valid && !f->m_destroyedObjects.contains(obj)) f->m_destroyObjects.push_back(obj);
    }
    
    if (f->m_checkCollisions > 0) return;
    
    int destroyedObjects = 0;
    
    for (GameObject* obj : f->m_destroyObjects) {
        if (!obj || f->m_destroyedObjects.contains(obj)) continue;
        float playerPos = 0.f;
        float objPos = 0.f;
        float half = 20 * f->m_scaleMultiplier;
        
        if (f->m_collisionDirection == PlayerCollisionDirection::Top || f->m_collisionDirection == PlayerCollisionDirection::Bottom) {
            objPos = obj->getPositionX();
            playerPos = player->getPositionX();
        } else {
            objPos = obj->getPositionY();
            playerPos = player->getPositionY();
        }
        
        if (!(objPos > playerPos - half && objPos < playerPos + half)) continue;
        
        if (f->m_collisionDirection == PlayerCollisionDirection::Bottom) {
            static std::mt19937 gen(std::random_device{}());
            static std::uniform_int_distribution<> dist(1, 100);
                                        
            int chance = 17;
            
            if (f->m_scaleMultiplier > 2.3f)
                chance += 5 + (f->m_scaleMultiplier - 2.3f) / 5.f * 25;
            
            if (dist(gen) > chance) continue;
        }
        
        f->m_destroyedObjects.insert(obj);
        destroyedObjects++;
                                
        AnimationManager::destroyObject(obj, 3, m_objectLayer, DestroyAnimation::Break);
    }
    
    if (destroyedObjects >= 8) shakeCamera(destroyedObjects / 10.f * 0.09f, destroyedObjects / 10.f * 0.09f, 0);
}

void ProGJBaseGameLayer::resetLevelVariables() {
    GJBaseGameLayer::resetLevelVariables();
    if (!LevelEditorLayer::get()) ProGJBaseGameLayer::resetYummyFieldsVariables();
}

void ProGJBaseGameLayer::resetYummyFieldsVariables() {
    auto f = m_fields.self();
    f->m_scaleMultiplier = 1.f;
    f->m_scaleMultiplierPlayerTwo = 1.f;
    f->m_checkCollisions = 0;
    f->m_isEating = false;
}

void ProGJBaseGameLayer::reactToPlayerScaleToggle(PlayerObject* player, bool toMini) {
    auto f = m_fields.self();
    if (player == m_player1) f->m_scaleMultiplier *= toMini ? .6f : (1.f / .6f);
    else if (player == m_player2) f->m_scaleMultiplierPlayerTwo *= toMini ? .6f : (1.f / .6f);
}