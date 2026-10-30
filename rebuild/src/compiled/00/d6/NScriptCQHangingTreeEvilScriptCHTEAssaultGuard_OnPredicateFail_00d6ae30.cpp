// NScript::CQ_HangingTreeEvilScript::CHTE_AssaultGuard::OnPredicateFail() -- an assault guard's predicate failed without it being killed: carry one more guard over
// Leaf: ?OnPredicateFail@CHTE_AssaultGuard@CQ_HangingTreeEvilScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6AE30, 60 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012D223C, which the entity Alloc 0x00D6AD70 (`NScript::CQ_HangingTreeEvilScript::CHTE_AssaultGuard::Alloc`) stores; ?OnPredicateFail@CHTE_AssaultGuard@CQ_HangingTreeEvilScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d6ae30" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::MsgIsKilledBy (slot 18, +0x48; CScriptThing vtable 0x01238C8C).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. Parent: CarryOverGuards +0x70 (Ego_r.pdb +0x84).
// Shape: retail keeps the call result in bl and jumps over the increment with `jne`: an empty then-arm with the increment in `else` (`if (!...)` gives `neg ; sbb ; inc`).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
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
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
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
class CQ_HangingTreeEvilScript {
public:
    class CHTE_AssaultGuard;
    unsigned char _pad000[0x70];
    long CarryOverGuards;                               // +0x70 (retail = Ego_r.pdb - 0x14)
};
class CQ_HangingTreeEvilScript::CHTE_AssaultGuard : public CEntityScriptBase {
public:
    CQ_HangingTreeEvilScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_HangingTreeEvilScript::CHTE_AssaultGuard::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
    } else {
        ++ParentClass->CarryOverGuards;
    }
}