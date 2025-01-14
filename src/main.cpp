/**
 * Small mod that randomizes gamemode on portal
 * TODO: MIGRATE ALL OF GJBaseGameLayer TO PLAYER OBJECT
 */
#include <Geode/Geode.hpp>
#include <random>
#include <queue>

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;

#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(randomizerGJBaseGameLayer, GJBaseGameLayer) {

    struct Fields {
        std::vector<GameObjectType> randomizedModes;
        bool checked;
        std::queue<EffectGameObject> x; 
        EffectGameObject* previousPortal = nullptr;
        GameObjectType previousOriginalType;

        void checkInit(PlayerObject* player) {
            if (this->checked) return;

            auto mod = Mod::get();
            if (mod->getSettingValue<bool>("cube")) this->randomizedModes.push_back(GameObjectType::CubePortal);
            if (mod->getSettingValue<bool>("ship")) this->randomizedModes.push_back(GameObjectType::ShipPortal);
            if (mod->getSettingValue<bool>("ball")) this->randomizedModes.push_back(GameObjectType::BallPortal);
            if (mod->getSettingValue<bool>("ufo")) this->randomizedModes.push_back(GameObjectType::UfoPortal);
            if (mod->getSettingValue<bool>("robot")) this->randomizedModes.push_back(GameObjectType::RobotPortal);
            if (mod->getSettingValue<bool>("spider")) this->randomizedModes.push_back(GameObjectType::SpiderPortal);
                if (mod->getSettingValue<bool>("swing")) this->randomizedModes.push_back(GameObjectType::SwingPortal);

            // This breaks the order yes but it looks nicer idk :(
            if ((player->m_isPlatformer && mod->getSettingValue<bool>("platModes")) || !player->m_isPlatformer) {
                log::debug("x");
                if (mod->getSettingValue<bool>("wave")) this->randomizedModes.push_back(GameObjectType::WavePortal);
            }
            // for (int i = 0; i < this->randomizedModes.size(); i++) {
                
            // }
            log::debug("size: {}", this->randomizedModes.size());

            this->checked = true;
        }

        void resetPreviousPortal() {
            if (previousPortal != nullptr) {
                previousPortal->m_objectType = previousOriginalType;
            }

            previousPortal = nullptr;
        }

        void resetPreviousPortal(EffectGameObject* newPortal, GameObjectType newID) {
            if (previousPortal != nullptr) {
                previousPortal->m_objectType = previousOriginalType;
            }

            previousPortal = newPortal;
            previousOriginalType = newID;
        }
    };

    bool init() {
        if (!GJBaseGameLayer::init()) return false;
        m_fields->checked = !Mod::get()->getSettingValue<bool>("enabled");
        return true;
    }

    // uses a random portal if portal is activated
    bool canBeActivatedByPlayer(PlayerObject* player, EffectGameObject* portal) {

        if (Mod::get()->getSettingValue<bool>("enabled") && isFormatEnabled(player->m_isPlatformer) && isGameMode(portal->m_objectType)) {
            m_fields->checkInit(player);
            bool x = GJBaseGameLayer::canBeActivatedByPlayer(player, portal);
            // portal->m_objectType = getRandomPortal();

            // player->switchedToMode(getRandomPortal());
            // player->toggleRollMode(false, false);
            // player->toggleDartMode(true, true);
            // log::debug("x");

            return x; 

            // if (m_fields->randomizedModes.size() > 0) {
            //     auto id = portal->m_objectType;
            //     // EffectGameObject* id = static_cast<EffectGameObject*>(portal->copy());
            //     portal->m_objectType = getRandomPortal(); // changes the type to now selected type
            //     m_fields->resetPreviousPortal(portal, id);
            // }             
        }

        return GJBaseGameLayer::canBeActivatedByPlayer(player, portal);

    }

    void resetPlayer() {
        m_fields->resetPreviousPortal();
        GJBaseGameLayer::resetPlayer();
    }

    bool isFormatEnabled(bool isPlat) {
        if (isPlat) {
            return Mod::get()->getSettingValue<bool>("platformer");
        } else {
            return Mod::get()->getSettingValue<bool>("classic");
        }
    }

    GameObjectType getRandomPortal() { 
        int randomVal = rand() % m_fields->randomizedModes.size();
        return m_fields->randomizedModes[randomVal];
    }

    bool isGameMode(GameObjectType type) {
        return type == GameObjectType::CubePortal || type == GameObjectType::ShipPortal
            || type == GameObjectType::BallPortal || type == GameObjectType::UfoPortal  
            || type == GameObjectType::WavePortal || type == GameObjectType::RobotPortal 
            || type == GameObjectType::SpiderPortal || type == GameObjectType::SwingPortal;
    }
};

#include <Geode/modify/PlayerObject.hpp>
class $modify(eeee, PlayerObject) {
    void switchedToMode(GameObjectType p0) {
        if (auto layer = static_cast<randomizerGJBaseGameLayer*>(m_gameLayer)) {
            if (layer->m_fields->randomizedModes.size() > 0) {
                
                p0 = layer->getRandomPortal();
                PlayerObject::switchedToMode(p0);

            switch (p0) {
                        // 0-7 gamemode actions
                case GameObjectType::CubePortal: {
                    toggleFlyMode(false, false);
                    toggleRollMode(false, false);
                    toggleBirdMode(false, false);
                    toggleDartMode(false, false);
                    toggleRobotMode(false, false);
                    toggleSpiderMode(false, false);
                    toggleSwingMode(false, false);
                    break;
                }
                case GameObjectType::ShipPortal: {
                    toggleFlyMode(true, true);
                    break;
                }

                case GameObjectType::BallPortal: {
                    toggleRollMode(true, true);
                    break;
                }
                case GameObjectType::UfoPortal: {
                    toggleBirdMode(true, true);
                    break;
                }
                case GameObjectType::WavePortal: {
                    toggleDartMode(true, true);
                    break;
                }
                case GameObjectType::RobotPortal: {
                    toggleRobotMode(true, true);
                    break;
                }
                case GameObjectType::SpiderPortal: {
                    toggleSpiderMode(true, true);
                    break;
                }
                case GameObjectType::SwingPortal: {
                    toggleSwingMode(true, true);
                    break;
                }
                default: {
                    log::debug("a");
                }
                    }
            }
        } else {
            PlayerObject::switchedToMode(p0);
        }
    }    
};