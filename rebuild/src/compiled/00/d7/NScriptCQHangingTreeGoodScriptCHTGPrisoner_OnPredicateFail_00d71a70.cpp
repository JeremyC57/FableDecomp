// NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::OnPredicateFail() -- when the prisoner dies: killed by the hero sets HeroKilledPrisoner and MissionSucceeded, by the executioner MissionSucceeded, by anything else MissionFailed
// Leaf: ?OnPredicateFail@CHTG_Prisoner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D71A70, 182 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D28A0, which the entity Alloc 0x00D71980 (`NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::Alloc`) stores; ?OnPredicateFail@CHTG_Prisoner@CQ_HangingTreeGoodScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d71a70" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptThing::MsgIsKilledBy (slot 18, +0x48).
// Data: "SCRIPT_NAME_HERO" @ 0x0125D1C8, "HTG_Executioner" @ 0x012D2698, the pooled empty string @ 0x0122D70E.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). Parent MissionSucceeded +0x4a, MissionFailed +0x4b, HeroKilledPrisoner +0x51 (Ego_r.pdb +0x5e/+0x5f/+0x65; same spellings as the landed CQ_HangingTreeGoodScript rows).
// Shape: three guarded returns; the constant true is loaded once into al for the first pair of stores.
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
class CQ_HangingTreeGoodScript {
public:
    unsigned char _pad000[0x4a];
    bool MissionSucceeded;                              // +0x4a (Ego_r.pdb +0x5e)
    bool MissionFailed;                                 // +0x4b
    unsigned char _pad04c[0x5];
    bool HeroKilledPrisoner;                            // +0x51 (Ego_r.pdb +0x65)
    class CHTG_Prisoner;
};
class CQ_HangingTreeGoodScript::CHTG_Prisoner : public CEntityScriptBase {
public:
    CQ_HangingTreeGoodScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    CScriptThing              ChopBlockMarker;            // +0x1c
    bool                      ReturningToParty;           // +0x28
    bool                      GuardsCalledAfterMe;        // +0x29
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_Prisoner::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO")) {
        ParentClass->HeroKilledPrisoner = true;
        ParentClass->MissionSucceeded = true;
        return;
    }
    if (Me.MsgIsKilledBy("HTG_Executioner")) {
        ParentClass->MissionSucceeded = true;
        return;
    }
    if (Me.MsgIsKilledBy(""))
        ParentClass->MissionFailed = true;
}