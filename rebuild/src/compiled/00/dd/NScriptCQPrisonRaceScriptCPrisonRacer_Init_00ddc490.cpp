// NScript::CQ_PrisonRaceScript::CPrisonRacer::Init() -- RaceWaypointIndex = 0; make the racer unkillable and give it a green mini-map orb
// Leaf: ?Init@CPrisonRacer@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDC490, 77 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012DCA2C, which the entity Alloc 0x00DDC3E0 (`NScript::CQ_PrisonRaceScript::CPrisonRacer::Alloc`) stores; ?Init@CPrisonRacer@CQ_PrisonRaceScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00ddc490" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 517 EntitySetAsKillable(thing, false, true), 350 MiniMapAddMarker.
// Data: "HUD_ORB_GREEN_SMALL" @ 0x012C7F50.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CPrisonRacer: RaceWaypointIndex +0x1c.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonRaceScript {
public:
    class CPrisonRacer;
};
class CQ_PrisonRaceScript::CPrisonRacer : public CEntityScriptBase {
public:
    CQ_PrisonRaceScript*      ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      RaceWaypointIndex;          // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_PrisonRaceScript::CPrisonRacer::Init()
{
    RaceWaypointIndex = 0;
    Game->EntitySetAsKillable(Me, false, true);
    Game->MiniMapAddMarker(Me, "HUD_ORB_GREEN_SMALL");
}