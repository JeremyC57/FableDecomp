// NScript::CQ_EndGameScript::CGatewayUndead::OnPredicateFail() -- when this gateway undead is killed, raise the parent's undead bar by RaiseUndeadBarAmount times 3 below 50, 2 below 250, 1 below 500 and 0.5 above
// Leaf: ?OnPredicateFail@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D29320, 175 bytes, thiscall, plain `ret`.
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less vtable 0x012CBC74, which the entity Alloc 0x00D29230 (`NScript::CQ_EndGameScript::CGatewayUndead::Alloc`) stores; ?OnPredicateFail@CGatewayUndead@CQ_EndGameScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d29320" is a gap-scan placeholder.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CScriptThing::MsgIsKilledBy (slot 18, +0x48) with the pooled empty string 0x0122D70E.
// Data: float pool 50.0 @ 0x0124E7F8, 3.0 @ 0x0122DED4, 250.0 @ 0x0125A278, 500.0 @ 0x01237584, double 0.5 @ 0x012316F0.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets; parent members sit 0x14 below Ego_r (retail CScriptBase is 0x40). CGatewayUndead: RaiseUndeadBarAmount +0x1c; parent UndeadBar +0x48 (Ego_r.pdb +0x5c).
// Shape: `x * 2.0f` is the `fadd st,st`; the last factor is a double literal (fmul qword).
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
class CQ_EndGameScript {
public:
    unsigned char _pad000[0x48];
    float UndeadBar;                                    // +0x48 (Ego_r.pdb +0x5c)
    class CGatewayUndead;
};
class CQ_EndGameScript::CGatewayUndead : public CEntityScriptBase {
public:
    CQ_EndGameScript*         ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    float                     RaiseUndeadBarAmount;       // +0x1c
private:
    virtual void OnPredicateFail();
};
}

void NScript::CQ_EndGameScript::CGatewayUndead::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        if (ParentClass->UndeadBar < 50.0f)
            ParentClass->UndeadBar += RaiseUndeadBarAmount * 3.0f;
        else if (ParentClass->UndeadBar < 250.0f)
            ParentClass->UndeadBar += RaiseUndeadBarAmount * 2.0f;
        else if (ParentClass->UndeadBar < 500.0f)
            ParentClass->UndeadBar += RaiseUndeadBarAmount;
        else
            ParentClass->UndeadBar += RaiseUndeadBarAmount * 0.5;
    }
}