// NScript::CQ_WaspBossScript::CWaspAttacker::Main() -- entity script: register an "is alive" predicate on Me, yield, make Me attack the WaspVictim until the victim dies, then make Me attack the hero
// Leaf: ?Main@CWaspAttacker@CQ_WaspBossScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Naming wave 35: identity reg 00e0ec8b "WaspAttacker" in 00e0ea40; Alloc 00e11380; vtable 012e0474 slot 1; PDB ?Main@CWaspAttacker@CQ_WaspBossScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CWaspAttacker::Main (wave3/labels_alloc.tsv); PDB ?Main@CWaspAttacker@CQ_WaspBossScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CWaspAttacker@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
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
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                  // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing m_thing;                               // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};
inline bool IsAlive(const CScriptThing& thing) { return thing.IsAlive(); }
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_WaspBossScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CWaspAttacker;
};
}
using NScript::CQ_WaspBossScript;
namespace NScript {
class CQ_WaspBossScript::CWaspAttacker : public CEntityScriptBase {
public:
        CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
private:
    virtual void Main();                          // PDB ?Main@CWaspAttacker@CQ_WaspBossScript@NScript@@EAEXXZ
};
}
void NScript::CQ_WaspBossScript::CWaspAttacker::Main()
{
    {
        CIsThingAlive predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    CScriptThing victim = Game->GetThingWithScriptName("WaspVictim");
    Game->GiveThingBestEnemyTarget(Me, victim);
    while (reinterpret_cast<const CScriptThingView&>(victim).Thing().IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
}