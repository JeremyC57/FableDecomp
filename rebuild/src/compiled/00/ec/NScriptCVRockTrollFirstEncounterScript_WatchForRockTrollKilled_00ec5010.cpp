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
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data (CScriptThing vtable layout)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;    // slot 18 (+0x48)
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72();
    virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                                   // slot 75 (+0x12c)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing() {}                                      // out of line 0x004AA840
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
    bool MsgIsKilledBy(const CCharString& killer) const     // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
};
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
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    void SetPredicate(const NScript::CPredicateBase& predicate);   // 0x00CB7920
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
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