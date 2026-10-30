// CProximityPodium::Main() -- entity script: register an alive predicate over the podium, then each time the hero comes within 2 units switch the podium's activation trigger on for ten seconds and off again, until the script is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFB360, length 256 bytes, plain `ret` (entity-script vtable slot at
// 0x012F6B24).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's member), CEntityScriptBase::SetPredicate
// 0x00F35B10 and IsActiveThreadTerminating 0x00F35B30 (both on `this`; manifest labels propagated), ~CIsThingAlive
// 0x00EF7AB0 (out of line), IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float)
// (`__fastcall`, 0x00CBE2FF), CScriptThing::SetActivationTriggerStatus (virtual slot 69, +0x114); through the entity's
// game script interface (+4): NewScriptFrame (slot 7, +0x1c), GetHero (slot 70, +0x118), Pause(float) (slot 376, +0x5e0).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}.
// Shape: the landed CHTE_Prisoner::Main model (0x00D69F50): the predicate is a scoped frame object; `if (done) return;`
// then `while (!done) { while (!near) { yield; if (done) return; } if (done) return; ...; yield; }` -- both loops
// rotated, the inner test duplicated.
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                           // slot 0
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void SetActivationTriggerStatus(bool on);  // slot 69 (+0x114), 0x004AB0A0
};

namespace N_00ef7ab0 { struct CIsThingAlive {                                 // vtable 0x012C2FE8: a predicate holding a script thing
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                  // 0x00EF7AB0
}; } using N_00ef7ab0::CIsThingAlive;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CProximityPodium {
public:
    void*                 __vftable;                 // +0x00
    CGameScriptInterface* Game;                      // +0x04
    CScriptThing          Me;                        // +0x08
    void SetPredicate(CIsThingAlive& predicate);     // 0x00F35B10
    bool IsActiveThreadTerminating();                // 0x00F35B30
    void Main();
};

void CProximityPodium::Main()
{
    {
        CIsThingAlive predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Me.SetActivationTriggerStatus(true);
        Game->Pause(10.0f);
        Me.SetActivationTriggerStatus(false);
        Game->NewScriptFrame();
    }
}