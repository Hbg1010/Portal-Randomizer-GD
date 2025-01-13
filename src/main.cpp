/**
 * 
 * 
 */
#include <Geode/Geode.hpp>
#include <random>

/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;


#include <Geode/modify/GJBaseGameLayer.hpp>
class $modify(randomizerGJBaseGameLayer, GJBaseGameLayer) {

    struct Fields {
        std::vector<GameObjectType> randomizedModes;

        void constructSet() {
            // todo optimize this :)
            // todo: add options for what can be randomized
            if (Mod::get()->getSettingValue<bool>("cube")) this->randomizedModes.push_back(GameObjectType::CubePortal);
            if (Mod::get()->getSettingValue<bool>("ship")) this->randomizedModes.push_back(GameObjectType::ShipPortal);
            if (Mod::get()->getSettingValue<bool>("ball")) this->randomizedModes.push_back(GameObjectType::BallPortal);
            if (Mod::get()->getSettingValue<bool>("ufo")) this->randomizedModes.push_back(GameObjectType::UfoPortal);
            if (Mod::get()->getSettingValue<bool>("wave") && Mod::get()->getSettingValue<bool>("platModes")) this->randomizedModes.push_back(GameObjectType::WavePortal);
            if (Mod::get()->getSettingValue<bool>("robot")) this->randomizedModes.push_back(GameObjectType::RobotPortal);
            if (Mod::get()->getSettingValue<bool>("spider")) this->randomizedModes.push_back(GameObjectType::SpiderPortal);
            if (Mod::get()->getSettingValue<bool>("swing") && Mod::get()->getSettingValue<bool>("platModes")) this->randomizedModes.push_back(GameObjectType::SwingPortal);
            // log::debug("created set of size", this->randomizedModes.size());
        }
    };

    bool init() {
        if (!GJBaseGameLayer::init()) return false;
            if (Mod::get()->getSettingValue<bool>("enabled")) {
                m_fields->constructSet();
            }
        return true;
    }

    // uses a random portal if portal is activated
    bool canBeActivatedByPlayer(PlayerObject* p0, EffectGameObject* p1) {
        if (Mod::get()->getSettingValue<bool>("enabled") && isFormatEnabled(p0->m_isPlatformer) && isGameMode(p1->m_objectType)) {
            p1->m_objectType = getRandomPortal(); // changes the type to now selected type
        }

        return GJBaseGameLayer::canBeActivatedByPlayer(p0, p1);
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