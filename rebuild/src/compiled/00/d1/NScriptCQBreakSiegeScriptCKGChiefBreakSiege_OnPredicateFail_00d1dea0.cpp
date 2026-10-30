// NScript::CQ_BreakSiegeScript::CKG_ChiefBreakSiege::OnPredicateFail() -- when the chief was killed: the siege fails for reason 2 (unless it already succeeded) and the end cutscene is marked done
// Leaf: ?OnPredicateFail@CKG_ChiefBreakSiege@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D1DEA0, 87 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012CAF00, which the landed
//   NScript::CQ_BreakSiegeScript::CKG_ChiefBreakSiege::Alloc 0x00D1DDE0 stores (site 0x00D1DE16); Ego_r.pdb has
//   ?OnPredicateFail@CKG_ChiefBreakSiege@CQ_BreakSiegeScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the literal "" 0x0122D70E), CScriptThing::MsgIsKilledBy (virtual slot 18, +0x48).
// Layout: Ego_r.pdb CKG_ChiefBreakSiege: ParentClass +0x14, PMasterData +0x18; CQ_BreakSiegeScript (Ego_r - 0x14): MissionSucceeded +0x48,
//   MissionFailed +0x49, EndCutsceneDone +0x54, ReasonForFailure +0x58.
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
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48) 0x004AAAF0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BreakSiegeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool          MissionSucceeded;                     // +0x48
    bool          MissionFailed;                        // +0x49
    unsigned char _pad4a[0x54 - 0x4a];
    bool          EndCutsceneDone;                      // +0x54
    unsigned char _pad55[3];
    long          ReasonForFailure;                     // +0x58
    class CKG_ChiefBreakSiege;
};
class CQ_BreakSiegeScript::CKG_ChiefBreakSiege : public CEntityScriptBase {
public:
    CQ_BreakSiegeScript*           ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
private:
    virtual void OnPredicateFail();
};

void CQ_BreakSiegeScript::CKG_ChiefBreakSiege::OnPredicateFail()
{
    if (Me.MsgIsKilledBy(""))
    {
        ParentClass->ReasonForFailure = 2;
        if (!ParentClass->MissionSucceeded)
            ParentClass->MissionFailed = true;
        ParentClass->EndCutsceneDone = true;
    }
}
}