// NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::OnPredicateFail() -- when the imprisoned bandit leader was killed: the siege fails for reason 1 (unless it already succeeded)
// Leaf: ?OnPredicateFail@CImprisonedBanditLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1E140, 80 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012CAF38, which the landed
//   NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Alloc 0x00D1E090 stores (site 0x00D1E0C6); Ego_r.pdb has
//   ?OnPredicateFail@CImprisonedBanditLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the literal "" 0x0122D70E), CScriptThing::MsgIsKilledBy (virtual slot 18, +0x48).
// Layout: Ego_r.pdb CImprisonedBanditLeader: ParentClass +0x14, PMasterData +0x18; CQ_BreakSiegeScript (Ego_r - 0x14): MissionSucceeded +0x48,
//   MissionFailed +0x49, ReasonForFailure +0x58.
// Shape: the H035 OnPredicateFail shape (landed 0x00D1DD80).
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the virtual calls and the by-value copy need
//   a polymorphic local model (as the landed entity-script rows use).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
class CGameScriptInterfaceBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BreakSiegeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool          MissionSucceeded;                     // +0x48
    bool          MissionFailed;                        // +0x49
    unsigned char _pad4a[0x58 - 0x4a];
    long          ReasonForFailure;                     // +0x58
    class CImprisonedBanditLeader;
};
class CQ_BreakSiegeScript::CImprisonedBanditLeader : public CEntityScriptBase {
public:
    CQ_BreakSiegeScript*           ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
private:
    virtual void OnPredicateFail();
};

void CQ_BreakSiegeScript::CImprisonedBanditLeader::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
    {
        ParentClass->ReasonForFailure = 1;
        if (!ParentClass->MissionSucceeded)
            ParentClass->MissionFailed = true;
    }
}
}