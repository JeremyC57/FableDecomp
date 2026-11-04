// NScript::CQ_TentacleKrakenBossFightScript::CKrakenMinion::Main() -- kraken minion entity script: register the alive-filter, run a frame, then (unless done) keep trying to start scripting Me at priority 3, add the "HUD_ORB_QUEST_CORE" minimap marker and give Me the hero as its best enemy target
// Leaf: ?Main@CKrakenMinion@CQ_TentacleKrakenBossFightScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF4610, 281 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (NScript::CIsThingConscious filter, vtable 0x012C2FE8), filter registration
//   0x00F35B10 / IsDone 0x00F35B30 on `this` (propagated manifest labels), CBaseObject ctor 0x0099A380 (labelled
//   CBaseIntelligentPointer), the scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0
//   (labels HasPhysicsMesh / Clear / ~CScriptGameResourceObjectMovieBase are propagated), CCharString 0x0099EBF0 / 0x0099EAE0,
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 350 MiniMapAddMarker,
//   451 GiveThingBestEnemyTarget.
// Shape: the landed CFH_BriarRose / CV_ChickenKickingScript models: the handle is a 0x10-byte local (out-of-line base ctor,
//   own vtable store, PImp words zeroed through eax, out-of-line dtor on every exit after its construction); the start loop is
//   `while (!Start(...)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` (rotated); the marker name is an implicit conversion.
// Naming wave 35: identity reg 00df4289 "KrakenMinion" in 00df4240; Alloc 00df4520; vtable 012de590 slot 1; PDB ?Main@CKrakenMinion@CQ_TentacleKrakenBossFightScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CKrakenMinion::Main (wave3/labels_alloc.tsv); PDB ?Main@CKrakenMinion@CQ_TentacleKrakenBossFightScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CKrakenMinion@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                          // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
    virtual ~CScriptThing();                          // 0x004AA840
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGH = 3 };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CCountedPointer.h"
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
class CIsThingConscious : public CPredicateBase { public:                         // vtable 0x012C2FE8 (operator(): IsAlive() && !IsUnconscious(), CScriptThing slots 75/61)
    CScriptThing m_thing;                     // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
}
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_TentacleKrakenBossFightScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CKrakenMinion;
};
}
using NScript::CQ_TentacleKrakenBossFightScript;
namespace NScript {
class CQ_TentacleKrakenBossFightScript::CKrakenMinion : public CEntityScriptBase {
public:
        CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
private:
    virtual void Main();                          // PDB ?Main@CKrakenMinion@CQ_TentacleKrakenBossFightScript@NScript@@EAEXXZ
};
}

void NScript::CQ_TentacleKrakenBossFightScript::CKrakenMinion::Main()
{
    {
        NScript::CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating()) {
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    }
}