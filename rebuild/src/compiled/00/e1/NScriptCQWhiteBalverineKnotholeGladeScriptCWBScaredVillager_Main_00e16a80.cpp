// NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_ScaredVillager::Main() -- the White Balverine (Knothole Glade) villager script: keep the villager scripted while conscious, run one frame, then hold an empty scripted-thing handle until the thread is told to stop
// Leaf: ?Main@CWB_ScaredVillager@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line, as retail calls them here).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E16A80, length 199 bytes; vtable 0x012E0B28 slot 1 (Main), factory "WB_ScaredVillager" (0x00E16990,
// Ego_r.pdb 28 bytes). The catalog label CQ_HangingTreeEvilScript::BanditYells is propagated.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840, CEntityScriptBase::SetPredicate 0x00F35B10
// and CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (both on `this`; PDB names, landed callers also spell them
// RegisterFilter / IsDone), CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430, operator delete 0x00BFE9BC, and the
// script's game interface (+4) NewScriptFrame (slot 7).
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the CIsThingConscious predicate (vtable 0x012C2FE8, IsAlive && !IsUnconscious) is a scoped frame object, its vtable
// kept in edi for the ctor and the inline dtor store; the scripted-thing handle's inline ctor (CBaseObject call, own
// vtable, zeroed counted pointer) and implicit dtor (release, CScriptGameResourceObjectBase vtable, ~CBaseObject) bracket
// the rotated wait loop.
// Naming wave 35: identity reg 00e14188 "WB_ScaredVillager" in 00e13f10; Alloc 00e16990; vtable 012e0b28 slot 1; PDB ?Main@CWB_ScaredVillager@CQ_WhiteBalverineKnotholeGladeScript@NScript@@; refs=1; was under-qualified CWB_ScaredVillager::Main (wave3/labels_alloc.tsv); PDB ?Main@CWB_ScaredVillager@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ in FableWin.pdb.
//   Was `?Main@CWB_ScaredVillager@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
namespace NScript { class CPredicateBase; }
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NScript {
class CPredicateBase
{
public:
    virtual bool operator()() const;                     // slot 0 (0x00CDDAB0)
};
}
struct CIsThingConscious : public NScript::CPredicateBase {                               // NScript predicate, vtable 0x012C2FE8: Thing alive and not unconscious
    CScriptThing Thing;                                  // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
    ~CIsThingConscious() {}
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript { class CQ_WhiteBalverineKnotholeGladeScript; } using NScript::CQ_WhiteBalverineKnotholeGladeScript;
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase { public:                               // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    bool IsActiveThreadTerminating() const;              // 0x00F35B30 (PDB: const)
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_WhiteBalverineKnotholeGladeScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CWB_ScaredVillager;
};
}
namespace NScript {
class CQ_WhiteBalverineKnotholeGladeScript::CWB_ScaredVillager : public CEntityScriptBase {
public:
    CQ_WhiteBalverineKnotholeGladeScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*               PMasterData;   // +0x18

private:
    virtual void Main();                          // PDB ?Main@CWB_ScaredVillager@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
};
}

void NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_ScaredVillager::Main()
{
    {
        CIsThingConscious predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}