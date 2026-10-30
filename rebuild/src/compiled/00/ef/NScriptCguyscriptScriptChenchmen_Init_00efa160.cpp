// NScript::Cguy_scriptScript::Chenchmen::Init() -- make the henchman an ally of the hero, follow the hero at distance 1 and join FACTION_HERO
// Leaf: ?Init@Chenchmen@Cguy_scriptScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFA160, 115 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012F69B0, stored by the entity Alloc 0x00EFA0B0 (`NScript::Cguy_scriptScript::Chenchmen::Alloc`); ?Init@Chenchmen@Cguy_scriptScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00efa160" is a gap-scan placeholder.
// Callees: interface slots 70 GetHero, 599 EntitySetThingAsAllyOfThing, 229 EntityFollowThing, 601 EntitySetInFaction; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0. Data: "FACTION_HERO" @ 0x0126C3A4.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets (PMasterData is a Cjoss_test_scriptsData*).
// Shape: the outer call's vtable is read before the GetHero argument call and kept in ebx.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class Cjoss_test_scriptsData;
class Cguy_scriptScript {
public:
    class Chenchmen;
};
class Cguy_scriptScript::Chenchmen : public CEntityScriptBase {
public:
    Cguy_scriptScript*      ParentClass;                // +0x14
    Cjoss_test_scriptsData* PMasterData;                // +0x18
private:
    virtual void Init();
};
}

void NScript::Cguy_scriptScript::Chenchmen::Init()
{
    Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
    Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
    Game->EntitySetInFaction(Me, "FACTION_HERO");
}