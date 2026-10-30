// CV_RockTrollFirstEncounterScript::WatchForRockTrollKilled(CScriptThing) -- set an "is thing alive" predicate on the troll, yield a frame, wait until the troll has been killed (by anyone), then (unless the thread is terminating) remove the troll's health bar, hide the quest info and mark the mission over
// Leaf: ?WatchForRockTrollKilled@CV_RockTrollFirstEncounterScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC5010, 293 bytes (304 with padding), `ret 0Ch` (the by-value CScriptThing).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's copy), CScriptBase::SetPredicate(const
//   NScript::CPredicateBase&) 0x00CB7920, ~CIsThingAlive 0x00E241B0 (landed), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator
//   delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line); through the interface (+0x40) NewScriptFrame (slot 7),
//   DisplayQuestInfo (slot 321, +0x504), RemoveQuestInfoElement (slot 338, +0x548); the thing's implementation slot 18
//   MsgIsKilledBy (+0x48) behind the inlined null-safe wrapper. String "" 0x0122D70E.
// Shape: the predicate is a temporary CIsThingAlive (vtable 0x012C32F0 over a novtable CPredicateBase, CScriptThing
//   at +4) passed straight to SetPredicate and destroyed out of line; the by-value parameter is destroyed by this
//   function at three exits -- VC7.1 expands its inline destructor at the first (the yield macro's return) and calls
//   the out-of-line ~CScriptThing at the other two.
// Layout (retail = Ego_r.pdb - 0x14): MissionOver +0x4a, RockTrollHealthBarID +0x50.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (!PImp.Data) return false; return PImp.Data->MsgIsKilledBy(killer); }
namespace NScript {
class __declspec(novtable) CPredicateBase { public:               // vtable 0x012C2FF4 (never stored: novtable)
    virtual bool IsTrue() = 0;
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:            // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                       // 0x00E241B0
    virtual bool IsTrue();
};
}
using NScript::CIsThingAlive;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_RockTrollFirstEncounterScript : public CQuestScriptBase {
public:
    unsigned char _pad48[2];                                // +0x48 MissionSucceeded, MissionFailed
    bool          MissionOver;                              // +0x4a
    bool          TrollAlive;                               // +0x4b
    bool          TrollAwake;                               // +0x4c
    bool          Activated;                                // +0x4d
    bool          RockTrollTriggered;                       // +0x4e
    long          RockTrollHealthBarID;                     // +0x50
    void WatchForRockTrollKilled(CScriptThing troll);
};
} // namespace NScript
using NScript::CV_RockTrollFirstEncounterScript;
void NScript::CV_RockTrollFirstEncounterScript::WatchForRockTrollKilled(CScriptThing troll)
{
    SetPredicate(CIsThingAlive(troll));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!troll.MsgIsKilledBy("")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(RockTrollHealthBarID);
    Game->DisplayQuestInfo(false);
    MissionOver = true;
}