// NScript::CQ_HeroSoulsArenaScript::WatchArenaCellsToTurnOffVillage() -- Wait for region activation and update the target thing until cancellation.
// Leaf: ?WatchArenaCellsToTurnOffVillage@CQ_HeroSoulsArenaScript@NScript@@QAEXXZ
// Grade: RELOCATION_MATCH (wave-2 sec6: length from the retail bytes, BOUNDS_FIX/oracle_extra; moved onto the shared
//   retail NScript headers, bytes unchanged)
// Identity: Own Main callback registration00d7cfba stores this function with exact method literal WatchArenaCellsToTurnOffVillage and parent receiver; independent PDB nameset agrees. Own Game vtable01260f0c slot12 IsRegionLoaded,72 GetThingWithScriptName,440 SetVillageLimbo/447 SetThingAsUsable; real script base Game64. Counted handle CScriptThing lifetime and assigning returned handle explicit original copy semantics; late cancellation exits call destructor004aa840.
// Layout: actual typed script ancestry and original counted handle model; inherited Game64; inactive owned trailing quest state unaccessed. Shared headers retained as evidence, unused virtual slots descriptive.
// Headers: retail NScript models (engine/CScriptThing.h, CGameScriptInterfaceBase.h, CLevelScriptBase.h,
//   CCountedPointer.h, CBaseClass.h); the CScriptThing ctor/dtor/operator= are inline here (retail inlines them).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
__forceinline CScriptThing::CScriptThing() { }
__forceinline CScriptThing::~CScriptThing() { }
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void WatchArenaCellsToTurnOffVillage();
};
void CQ_HeroSoulsArenaScript::WatchArenaCellsToTurnOffVillage()
{
    CScriptThing thing;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsRegionLoaded("ArenaCells")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        thing = Game->GetThingWithScriptName("VILLAGE_ARENACELLS");
        Game->SetVillageLimbo(thing, true);
        while (Game->IsRegionLoaded("ArenaCells") != false) {
            Game->NewScriptFrame();
#pragma inline_depth(0)
            if (IsActiveThreadTerminating()) return;
#pragma inline_depth(255)
        }
#pragma inline_depth(0)
        if (IsActiveThreadTerminating()) return;
#pragma inline_depth(255)
        Game->NewScriptFrame();
    }
}
}