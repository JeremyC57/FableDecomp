// NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPointRear::Main() -- Hanging Tree (good) rear ambush point: wait until the quest's tracked thing comes within 15 units and then moves beyond 20, wait while this point is on screen and keep waiting (one-second pauses) until it is off screen; then have the quest spawn four enemies at the point with the point's data string (any wait bails out when the script is done)
// Leaf: Main
// Identity (naming wave 11): ?Main@CHTG_AmbushPointRear@CQ_HangingTreeGoodScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012D2754 slot 1 is stored by the factory 0x00D6FF30 registered as "HTG_AmbushPointRear"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D73AE0, length 386 bytes (the final IsDone is a tail `jmp`).
// Callees: IsDistanceBetweenThingsUnder @ 0x00CBE2FF / IsDistanceBetweenThingsOver @ 0x00CBE3EA (`__fastcall`,
//   (const CScriptThing&, const CScriptThing&, float)), CEntityScriptBase::IsDone @ 0x00F35B30 (manifest
//   "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated), CQ_HangingTreeGoodScript::SpawnEnemies(const
//   C3DVector&, CCharString, long) @ 0x00D736B0; CScriptThing (vtable 0x01238C8C) slots 3 GetDataString and 6 GetPos;
//   CGameScriptInterface (vtable 0x01260F0C) slots 7 NewScriptFrame, 376 Pause, 421 IsCameraPosOnScreen.
// Layout: as the landed sibling CHTG_AmbushPoint::Main 0x00D73610 (Ego_r.pdb CEntityScriptBase {vptr, Game +4,
//   CScriptThing Me +8}, ParentClass +0x14 whose tracked script thing sits at +0x68).
// Shape: the four waits are top-tested `while` loops (test repeated at the bottom) each followed by an IsDone check;
//   the outer `do { } while (on screen)` restarts with a script frame; the closing IsDone result is unused.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);
    ~CCharString();
};
class C3DVector { public: float x, y, z; };
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    unsigned char _pad[0x68];
    CScriptThing  m_trackedThing;                  // +0x68
    void SpawnEnemies(const C3DVector& position, CCharString name, long count);   // 0x00D736B0
public:
    class CHTG_AmbushPointRear;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_HangingTreeGoodScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_HangingTreeGoodScript::CHTG_AmbushPointRear : public CEntityScriptBase {
public:
        CQ_HangingTreeGoodScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_HangingTreeGoodScript::CHTG_AmbushPointRear::Main()
{
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        while (!IsDistanceBetweenThingsUnder(Me, ParentClass->m_trackedThing, 15.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!IsDistanceBetweenThingsOver(Me, ParentClass->m_trackedThing, 20.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (Game->IsCameraPosOnScreen(Me.GetPos())) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->Pause(1.0f);
    } while (Game->IsCameraPosOnScreen(Me.GetPos()));
    if (IsActiveThreadTerminating())
        return;
    ParentClass->SpawnEnemies(Me.GetPos(), Me.GetDataString(), 4);
    IsActiveThreadTerminating();
}