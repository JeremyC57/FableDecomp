// NScript::CQ_HangingTreeGoodScript::CHTG_Guard::OnPredicateFail() -- a guard's predicate failed: if killed, one guard fewer and HangingTreeGuardKilled; otherwise HTG_State = 1
// Leaf: ?OnPredicateFail@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D70650, 84 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D27A8, which the entity Alloc 0x00D70520 (`NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Alloc`) stores; ?OnPredicateFail@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d70650" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48; CScriptThing vtable 0x01238C8C).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. Parent (retail = Ego_r.pdb - 0x14): HTG_State +0x5c, CurrentGuards +0x64; CQ_SunnyvaleMasterData::HangingTreeGuardKilled +0x125.
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
class CGameScriptInterfaceBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeGoodScript {
public:
    class CHTG_Guard;
    unsigned char _pad000[0x5c];
    long HTG_State;                                     // +0x5c (retail = Ego_r.pdb - 0x14)
    unsigned char _pad060[0x4];
    long CurrentGuards;                                 // +0x64
};
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x125];
    bool HangingTreeGuardKilled;                        // +0x125 (Ego_r.pdb offset, same in retail)
};
class CQ_HangingTreeGoodScript::CHTG_Guard : public CEntityScriptBase {
public:
    CQ_HangingTreeGoodScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_Guard::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        --ParentClass->CurrentGuards;
        PMasterData->HangingTreeGuardKilled = true;
    } else {
        ParentClass->HTG_State = 1;
    }
}