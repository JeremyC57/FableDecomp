// NScript::CQ_SecretPassageScript::CSP_AmbushGuard::Main() -- the secret-passage ambush guard: alive filter on Me, take scripting control, wait until the hero sees or hears the guard, then have the guard shout and set the hero as his enemy
// Leaf: ?Main@CSP_AmbushGuard@CQ_SecretPassageScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DED540, length 470 bytes (padded to 480). Entity-script slot 1 (Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the CIsThingAlive filter, vtable
//   0x012C32F0), the filter registration @ 0x00F35B10, IsDone @ 0x00F35B30 (on `this`), CBaseObject ctor @ 0x0099A380,
//   the scripted-thing handle's IsValid @ 0x00CD23B9 / Release @ 0x00CD2770 / destructor @ 0x007E74D0 (vtable
//   0x0127094C); CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0; CGameScriptInterface slots
//   7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 364 AddNewConversation (+0x5b0), 366 AddLineToConversation
//   (+0x5b8), 451 GiveThingBestEnemyTarget (+0x70c), 632 CanThingBe_Seen_ByOtherThing (+0x9e0), 635
//   CanThingBe_Heard_ByOtherThing (+0x9ec).
// Layout: the entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_SecretPassageScript*), PMasterData +0x18.
// Shape: the landed CUndeadRisingZombieAttacker::Main (0x00E0CFF0) prelude and scripted-thing handle; the handle is
//   released again before its destructor at the end; the line text is passed straight to the `const CCharString&`.
// Naming wave 35: identity reg 00decb21 "SP_AmbushGuard" in 00decad0; Alloc 00ded450; vtable 012dddac slot 1; PDB ?Main@CSP_AmbushGuard@CQ_SecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CSP_AmbushGuard::Main (wave3/labels_alloc.tsv); PDB ?Main@CSP_AmbushGuard@CQ_SecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CSP_AmbushGuard@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic)); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                   // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript { class CQ_SecretPassageScript; } using NScript::CQ_SecretPassageScript;
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_SecretPassageScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CSP_AmbushGuard;
};
}
namespace NScript {
class CQ_SecretPassageScript::CSP_AmbushGuard : public CEntityScriptBase {
public:
        CGameScriptInterface*   Game;                        // +0x04
    CScriptThing            Me;                          // +0x08
    CQ_SecretPassageScript* ParentClass;                 // +0x14
    void*                   PMasterData;                 // +0x18
private:
    virtual void Main();                          // PDB ?Main@CSP_AmbushGuard@CQ_SecretPassageScript@NScript@@EAEXXZ
};
}

void NScript::CQ_SecretPassageScript::CSP_AmbushGuard::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!(Game->CanThingBe_Seen_ByOtherThing(Game->GetHero(), Me) || Game->CanThingBe_Heard_ByOtherThing(Game->GetHero(), Me))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    long conversation = Game->AddNewConversation(Me, false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_055_GUARD_SURPRISED_BY_HERO", false, Me, Game->GetHero());
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (handle.IsValid())
        handle.Release();
}