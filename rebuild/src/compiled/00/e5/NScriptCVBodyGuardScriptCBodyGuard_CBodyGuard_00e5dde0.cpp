// NScript::CV_BodyGuardScript::CBodyGuard::CBodyGuard(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me) -- a henchman's entity script: entity-script base over the quest's interface and Me, the parent, the anger and attacked-by-summoners timers, and the master data last
// Leaf: ??0CBodyGuard@CV_BodyGuardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5DDE0, 90 bytes, thiscall, `ret 0Ch`, returns this.
// Identity (proven): stores the CBodyGuard entity vtable 0x012E701C (site 0x00E5DE01), whose slot 5 is the landed
//   CBodyGuard::OnPredicateFail 0x00E5FB80 (hand508); only caller the "BodyGuard" binding's Alloc 0x00E5DD60
//   (call 0x00E5DD81). FableWin.pdb ??0CBodyGuard@CV_BodyGuardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
//   Label correction: the manifest row "CDarkwoodTrader::CDarkwoodTrader" is propagated.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, g_pGameScriptInterface (0x0143E8F8)->RegisterTimer
//   (slot 87, +0x15C) for each CTimer.
// Layout: Ego_r.pdb CBodyGuard (60 bytes, entity scripts keep their PDB offsets): CEntityScriptBase {vptr 0x012C3224,
//   Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, HenchManType +0x1c, BrainState +0x20, CurrentHealthState +0x24,
//   PreviousHealthState +0x28, FreakedOut +0x2c (all uninitialised), AngerTimer +0x30, AttackedBySummonersTimer +0x34,
//   MyHealthBarID +0x38.
// Shape: the landed CFisticuffsMember constructor 0x00E83920 model (Game a plain base initialiser stored before the base
//   vtable, the derived vtable before ParentClass, the timers, PMasterData a body assignment stored last).
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CScriptDataBase;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:            // vtable 0x01238C8C, 12 bytes (PImp Data +4, Info +8)
    CCountedPointerWords PImp;                // +0x04
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;               // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing          Me;                 // +0x08
    CEntityScriptBase(const CScriptThing& thing, CGameScriptInterface* game) : Game(game), Me(thing) { }
};
class CTimer { public:                               // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BodyGuardScript {                              // the owning level script: only Game is read
public:
    unsigned char         _pad00[0x40];                 // CScriptBase
    CGameScriptInterface* Game;                         // +0x40 (CLevelScriptBase<T>::Game)
    class CBodyGuard;
};
class CV_BodyGuardScript::CBodyGuard : public CEntityScriptBase {   // vtable 0x012E701C
public:
    CV_BodyGuardScript*     ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    HenchManType;               // +0x1c
    long                    BrainState;                 // +0x20
    long                    CurrentHealthState;         // +0x24
    long                    PreviousHealthState;        // +0x28
    bool                    FreakedOut;                 // +0x2c
    CTimer                  AngerTimer;                 // +0x30
    CTimer                  AttackedBySummonersTimer;   // +0x34
    long                    MyHealthBarID;              // +0x38
    virtual void v00();
    CBodyGuard(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
CV_BodyGuardScript::CBodyGuard::CBodyGuard(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(me, parent.Game), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}