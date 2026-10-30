// NScript::CQ_BanditCampScript::CGate1GuardOuter::Init() -- reset the outer gate guard: AIState 0, give it information unless gate 1 is open, put it in the Twinblade camp bandit faction, make it unpushable and clear the parent's AttackedOuterGateGuards
// Leaf: ?Init@CGate1GuardOuter@CQ_BanditCampScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D01590, 145 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012C8604, which the entity Alloc 0x00D014C0 (`NScript::CQ_BanditCampScript::CGate1GuardOuter::Alloc`) stores; ?Init@CGate1GuardOuter@CQ_BanditCampScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d01590" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; interface slots 360 SetThingHasInformation, 601 EntitySetInFaction, 844 SetIsPushableByHero (CScriptThing by value: CBaseClass then CScriptThing vtable built in the argument slot, PImp copied with its count bump).
// Data: "FACTION_TWINBLADE_CAMP_BANDITS" @ 0x012C8620.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). CGate1GuardOuter: AIState +0x1c; parent AttackedOuterGateGuards +0x48, Gate1Open +0x49.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampScript {
public:
    unsigned char _pad000[0x48];
    bool AttackedOuterGateGuards;                       // +0x48 (Ego_r.pdb +0x5c)
    bool Gate1Open;                                     // +0x49
    class CGate1GuardOuter;
};
class CQ_BanditCampScript::CGate1GuardOuter : public CEntityScriptBase {
public:
    CQ_BanditCampScript*      ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      AIState;                    // +0x1c
private:
    virtual void Init();
};
}

void NScript::CQ_BanditCampScript::CGate1GuardOuter::Init()
{
    AIState = 0;
    if (!ParentClass->Gate1Open)
        Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetInFaction(Me, "FACTION_TWINBLADE_CAMP_BANDITS");
    Game->SetIsPushableByHero(Me, false);
    ParentClass->AttackedOuterGateGuards = false;
}