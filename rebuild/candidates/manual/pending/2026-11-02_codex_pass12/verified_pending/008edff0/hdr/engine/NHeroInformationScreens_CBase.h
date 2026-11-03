#ifndef FABLE_ENGINE_NHEROINFORMATIONSCREENS_CBASE_H
#define FABLE_ENGINE_NHEROINFORMATIONSCREENS_CBASE_H
#include "engine/CWideString.h"
namespace NUISystem { class CComponent; }
enum EPlayerMode {  };
namespace NHeroInformationScreens {
enum EScreenType { NULL_SCREEN_TYPE=-1,STANDARD_INVENTORY_SCREEN=0,INVENTORY_QUESTS_SCREEN=1,INVENTORY_EXPERIENCE_SCREEN=2,HERO_ABILITIES_SCREEN=3,INVENTORY_WEAPONS_SCREEN=4,INVENTORY_CLOTHING_SCREEN=5,INVENTORY_TRADE_SCREEN=6,QUEST_CARD_SCREEN=7,INVENTORY_MAP_SCREEN=8,INVENTORY_STATS_SCREEN=9,INVENTORY_MAGIC_SCREEN=10,NUMBER_OF_SCREEN_TYPES=11 };
class CBase {
public: virtual CWideString GetScreenName() const;
public: virtual EPlayerMode GetPlayerMode() const;
public: virtual NHeroInformationScreens::EScreenType GetScreenType() const;
public: virtual bool IsInventoryScreen() const;
};
}
FABLE_STATIC_ASSERT(sizeof(NHeroInformationScreens::CBase)==4);
#endif
