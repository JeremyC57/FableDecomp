// NScript::CQ_WizardBattleScript::CHKC_ScreamerPoint::Main() -- a screamer spawn point: hold an alive filter over Me, wait until the quest arms the point, then until the point is switched off spawn a "CREATURE_SCREAMER_01" at Me whenever the hero comes within 20, set it on the hero and wait for it to die
// Leaf: ?Main@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1B750, 512 bytes; entity-script vtable slot 1 (Main), plain `ret`.
// Callees: CScriptThing copy ctor @ 0x004ABE90 / ~CScriptThing @ 0x004AA840, CEntityScriptBase::SetPredicate @ 0x00F35B10,
//          CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CBaseObject::CBaseObject @ 0x0099A380, the scripted-thing
//          handle destructor @ 0x007E74D0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0
//          ("CREATURE_SCREAMER_01" @ 0x012E1034, "SpawnedScreamer" @ 0x012E104C), IsDistanceBetweenThingsUnder @ 0x00CBE2FF
//          (__fastcall, distance on the stack); CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 91 CreateCreature
//          (by value), 451 GiveThingBestEnemyTarget; CScriptThing slots 6 GetPos and 75 IsAlive.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} and the scripted-thing handle (vtable 0x0127094C) as in the landed
//         CChasingScreamer::Main 0x00E1AF30 (same TU); this class adds the quest master data pointer +0x14 (armed flag +0x48,
//         stop flag +0x4f) and a dword +0x1c cleared after every spawn.
// Shape: the handle is declared (never used) at function scope after the first terminating test; the two creature-name
//        strings are implicit conversion temporaries built before the CreateCreature pushes (Me.GetPos() is evaluated
//        between the pushes); the spawned creature is a block-scoped by-value local.
// Naming wave 35: identity reg 00e1aa4a "HKC_ScreamerPoint" in 00e1a990; Alloc 00e1b660; vtable 012e1078 slot 1; PDB ?Main@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CHKC_ScreamerPoint::Main (wave3/labels_alloc.tsv); PDB ?Main@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CHKC_ScreamerPoint@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#include "engine/C3DVector.h"

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
struct CScriptThingView {
    void* vptr; void* Data; void* Info;
    const CScriptThing& Thing() const { return *reinterpret_cast<const CScriptThing*>(this); }
};

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
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
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                 // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;

class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }

struct CHKCMasterData {
    unsigned char _pad00[0x48];
    bool          ScreamersArmed;                      // +0x48
    unsigned char _pad49[0x4f - 0x49];
    bool          ScreamersStopped;                    // +0x4f
};

namespace NScript {
class CQ_WizardBattleScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CHKC_ScreamerPoint;
};
}
using NScript::CQ_WizardBattleScript;
namespace NScript {
class CQ_WizardBattleScript::CHKC_ScreamerPoint : public CEntityScriptBase {
public:
    CHKCMasterData* PMasterData;                       // +0x14
    unsigned char   _pad18[4];                         // +0x18
    long            SpawnTimer;                        // +0x1c
private:
    virtual void Main();                          // PDB ?Main@CHKC_ScreamerPoint@CQ_WizardBattleScript@NScript@@EAEXXZ
};
}

void NScript::CQ_WizardBattleScript::CHKC_ScreamerPoint::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
    while (!PMasterData->ScreamersArmed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!PMasterData->ScreamersStopped) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 20.0f)) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing screamer = Game->CreateCreature("CREATURE_SCREAMER_01", Me.GetPos(), "SpawnedScreamer", true);
            SpawnTimer = 0;
            Game->GiveThingBestEnemyTarget(screamer, Game->GetHero());
            while (reinterpret_cast<const CScriptThingView&>(screamer).Thing().IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    IsActiveThreadTerminating();
}