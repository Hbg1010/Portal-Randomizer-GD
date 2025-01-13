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
        std::unordered_set<GameObjectType> randomizedModes;

        void constructSet() {
            // todo optimize this :)
            // todo: add options for what can be randomized
            if (Mod::get()->getSettingValue<bool>("cube")) this->randomizedModes.insert(GameObjectType::CubePortal);
            if (Mod::get()->getSettingValue<bool>("ship")) this->randomizedModes.insert(GameObjectType::ShipPortal);
            if (Mod::get()->getSettingValue<bool>("ball")) this->randomizedModes.insert(GameObjectType::BallPortal);
            if (Mod::get()->getSettingValue<bool>("ufo")) this->randomizedModes.insert(GameObjectType::UfoPortal);
            if (Mod::get()->getSettingValue<bool>("wave") && Mod::get()->getSettingValue<bool>("platModes")) this->randomizedModes.insert(GameObjectType::WavePortal);
            if (Mod::get()->getSettingValue<bool>("robot")) this->randomizedModes.insert(GameObjectType::RobotPortal);
            if (Mod::get()->getSettingValue<bool>("spider")) this->randomizedModes.insert(GameObjectType::SpiderPortal);
            if (Mod::get()->getSettingValue<bool>("swing") && Mod::get()->getSettingValue<bool>("platModes")) this->randomizedModes.insert(GameObjectType::SwingPortal);

            log::debug("created set of size", this->randomizedModes.size());
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

    bool isFormatEnabled(bool& isPlat) {
        if (isPlat) {
            return Mod::get()->getSettingValue<bool>("platformer");
        } else {
            return Mod::get()->getSettingValue<bool>("classic");
        }
    }

    GameObjectType getRandomPortal() {
        int randomVal = rand() % m_fields->randomizedModes.size();
        auto current = m_fields->randomizedModes.begin();
        for (int i = 0; i < randomVal; i++) {
            current++;
        }

        return *current;
    }

    bool isGameMode(GameObjectType type) {
        return type == GameObjectType::CubePortal || type == GameObjectType::ShipPortal
            || type == GameObjectType::BallPortal || type == GameObjectType::UfoPortal  
            || type == GameObjectType::WavePortal || type == GameObjectType::RobotPortal 
            || type == GameObjectType::SpiderPortal || type == GameObjectType::SwingPortal;
    }
};

// #include <Geode/modify/PlayerObject.hpp>
// class $modify(RandomizerPlayerObj, PlayerObject) {

//     struct Fields {
//         std::unordered_set<GameObjectType> randomizedModes;

//         void constructSet() {
//             // todo optimize this :)
//             // todo: add options for what can be randomized
//             this->randomizedModes.insert(GameObjectType::CubePortal);
//             this->randomizedModes.insert(GameObjectType::ShipPortal);
//             this->randomizedModes.insert(GameObjectType::BallPortal);
//             this->randomizedModes.insert(GameObjectType::UfoPortal);
//             this->randomizedModes.insert(GameObjectType::WavePortal);
//             this->randomizedModes.insert(GameObjectType::RobotPortal);
//             this->randomizedModes.insert(GameObjectType::SpiderPortal);
//             this->randomizedModes.insert(GameObjectType::SwingPortal);

//             log::debug("created set!");
//         }
//     };

//     bool init(int p0, int p1, GJBaseGameLayer* p2, cocos2d::CCLayer* p3, bool p4) {
//         if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;
//         log::debug("{}",m_mainLayer->getID());
//         // if (m_mainLayer->getID()); // figure out if this can be obtained without ID
//         m_fields->constructSet();
//         return true;
//     }
//     // void switchedToMode(GameObjectType obj) {
//     //     if (m_fields->randomizedModes.size() > 0) {
//     //         PlayerObject::switchedToMode(obj);

//     //         if (m_fields->randomizedModes.find(obj) != m_fields->randomizedModes.end()) {
//     //             log::debug("Hit a portal!");
//     //             modeRandomizer();
//     //         }
//     //     }

//     // }

//     GameObjectType modeRandomizer() {
//         auto Fields = m_fields.self();
//         int randomVal = rand() % Fields->randomizedModes.size();
//         auto current = Fields->randomizedModes.begin();
//         for (int i = 0; i < randomVal; i++) {
//             current++;
//         }

//         log::debug("{}", randomVal);
//         log::debug("hello world");
//         switch(randomVal) {
//             case -1: break;
            
//             // 0-7 gamemode actions
//             case 0: {
//                 toggleFlyMode(false, false);
//                 toggleRollMode(false, false);
//                 toggleBirdMode(false, false);
//                 toggleDartMode(false, false);
//                 toggleRobotMode(false, false);
//                 toggleSpiderMode(false, false);
//                 toggleSwingMode(false, false);
//                 break;
//             }
//             case 1: {
//                 toggleFlyMode(true, true);
//                 break;
//             }

//             case 2: {
//                 toggleRollMode(true, true);
//                 break;
//             }
//             case 3: {
//                 toggleBirdMode(true, true);
//                 break;
//             }
//             case 4: {
//                 PlayerObject::switchedToMode(GameObjectType::WavePortal);
//                 toggleDartMode(true, true);
//                 break;
//             }
//             case 5: {
//                 toggleRobotMode(true, true);
//                 break;
//             }
//             case 6: {
//                 toggleSpiderMode(true, true);
//                 break;
//             }
//             case 7: {
//                 toggleSwingMode(true, true);
//                 PlayerObject::switchedToMode(GameObjectType::SwingPortal);


//                 break;
//             }

//             default: {
//                 log::debug("there was an error!");
//                 break;
//             }
//               auto obj = TeleportPortalObject::create("edit_eGameRotBtn_001.png", true);
//             obj->m_cameraIsFreeMode = true;
//             GJBaseGameLayer::get()->playerWillSwitchMode(this, obj);
//         }

//         return *current;

//     }

//     void forceMode(int gameMode) {
//         switch (gameMode) {
//         case -1:
//             /* code */
//             break;
        
//         default:
//             break;
//         }
//     }
// };

