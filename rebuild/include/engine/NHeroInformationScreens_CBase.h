#ifndef FABLE_ENGINE_NHEROINFORMATIONSCREENS_CBASE_H
#define FABLE_ENGINE_NHEROINFORMATIONSCREENS_CBASE_H
#include "fable_string.h"
namespace NUISystem { class CComponent; }
// Codex pass 13 landing: the full original EPlayerMode (both PDBs) lives in engine/EPlayerMode.h; this header used to declare it empty.
#include "engine/EPlayerMode.h"
namespace NHeroInformationScreens {
enum EScreenType { NULL_SCREEN_TYPE=-1,STANDARD_INVENTORY_SCREEN=0,INVENTORY_QUESTS_SCREEN=1,INVENTORY_EXPERIENCE_SCREEN=2,HERO_ABILITIES_SCREEN=3,INVENTORY_WEAPONS_SCREEN=4,INVENTORY_CLOTHING_SCREEN=5,INVENTORY_TRADE_SCREEN=6,QUEST_CARD_SCREEN=7,INVENTORY_MAP_SCREEN=8,INVENTORY_STATS_SCREEN=9,INVENTORY_MAGIC_SCREEN=10,NUMBER_OF_SCREEN_TYPES=11 };
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_ORIGINAL_MEMBERS_PASS22
// Both original PDBs:7 real methods; default ctor/dtor user0x3, copy/assignment generated.
class CBase {
public:
 CBase();
 ~CBase();
 void Copy(const CBase&);
 virtual CWideString GetScreenName() const=0;
 virtual EPlayerMode GetPlayerMode() const=0;
 virtual EScreenType GetScreenType() const=0;
 virtual bool IsInventoryScreen() const;
};
#else
class CBase {
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_PASS22_DTOR
// FableWin01B9B84C original nonvirtual destructor; native0072DC27 restores this base vtable.
public: ~CBase() {}
#endif
#ifdef FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_PASS22_DTOR
// Native0123A3E0 slots0..2 are pure; the original secondary base is abstract.
public: virtual CWideString GetScreenName() const = 0;
public: virtual EPlayerMode GetPlayerMode() const = 0;
public: virtual NHeroInformationScreens::EScreenType GetScreenType() const = 0;
#else
public: virtual CWideString GetScreenName() const;
public: virtual EPlayerMode GetPlayerMode() const;
public: virtual NHeroInformationScreens::EScreenType GetScreenType() const;
#endif
public: virtual bool IsInventoryScreen() const;
};
#endif
}
FABLE_STATIC_ASSERT_AT(16, sizeof(NHeroInformationScreens::CBase)==4);
#endif
