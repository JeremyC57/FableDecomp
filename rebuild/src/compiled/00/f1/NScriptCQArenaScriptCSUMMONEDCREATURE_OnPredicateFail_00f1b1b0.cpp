// NScript::CQ_ArenaScript::CSUMMONED_CREATURE::OnPredicateFail() -- one summoned creature fewer in the arena; if the hero killed it, 8 more crowd points
// Leaf: ?OnPredicateFail@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F1B1B0, 80 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F9CC8, stored by the entity Alloc 0x00F1B0F0 (`NScript::CQ_ArenaScript::CSUMMONED_CREATURE::Alloc`); ?OnPredicateFail@CSUMMONED_CREATURE@CQ_ArenaScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00f1b1b0" is a gap-scan placeholder.
// Callees: CScriptThing::MsgIsKilledBy (slot 18, +0x48); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0. Data: "SCRIPT_NAME_HERO" @ 0x0125D1C8.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r.pdb offsets; parent CQ_ArenaScript (retail = Ego_r.pdb - 0x18 after the 12-byte Rounds vector, as the landed 0x00F25840): TotalCreatures[3] +0xd0, NewCrowdPoints +0xf4.
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString();                                        // 0x0099E4B0
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
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
class CQ_ArenaScript {
public:
    class CSUMMONED_CREATURE;
    unsigned char _pad000[0xd0];
    long TotalCreatures[3];                             // +0xd0 (Ego_r.pdb +0xe8)
    unsigned char _pad0dc[0x18];
    long NewCrowdPoints;                                // +0xf4 (Ego_r.pdb +0x10c)
};
class CQ_ArenaScript::CSUMMONED_CREATURE : public CEntityScriptBase {
public:
    CQ_ArenaScript*         ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_ArenaScript::CSUMMONED_CREATURE::OnPredicateFail()
{
    --ParentClass->TotalCreatures[0];
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO"))
        ParentClass->NewCrowdPoints += 8;
}