// NScript::CPersonalScript_EmptyScript::CWalker::Main() -- Cham's walk between the two map markers: hold an alive filter over Me, look up "InMap1" / "InMap2", make Me scared, take Me under scripted control (priority 4, retried each frame), move to InMap1, then keep following the precalculated routes to InMap1 and InMap2 every frame
// Leaf: ?Main@CWalker@CPersonalScript_EmptyScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF3AA0, 444 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor @ 0x004ABE90 / ~CScriptThing @ 0x004AA840, CEntityScriptBase::SetPredicate @ 0x00F35B10,
//          ~CIsThingAlive @ 0x00E241B0 (out of line in this module), CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30,
//          CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CBaseObject::CBaseObject @ 0x0099A380,
//          CScriptGameResourceObjectScriptedThingBase IsValid @ 0x00CD23B9 / Release @ 0x00CD2770 (propagated labels, as in
//          the landed 0x00EB6200), MoveToThing @ 0x007E7300 (slot 5) and FollowPreCalculatedRoute @ 0x007E7310 (slot 6) called
//          directly on the local, its destructor @ 0x007E74D0; CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity,
//          72 GetThingWithScriptName (by value), 496 EntitySetAsScared (ghidra_out/cgsi_slots_aligned.tsv).
//          Literals "InMap1" @ 0x012F6410, "InMap2" @ 0x012F6408.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} as in the landed entity Mains; the scripted-thing handle (vtable
//         0x0127094C, 0x10 bytes) as in the landed CSummonerToKill::Main 0x00D7DD80.
// Shape: the alive-filter prelude with the out-of-line filter destructor; the two markers are by-value function-scope
//        locals built from implicit-conversion name temporaries; the handle follows them; the follow loop is a rotated
//        `while (!IsActiveThreadTerminating())`. MoveToThing's 0.0f distance is the `push 0`.
// Naming wave 35: identity reg 00ef0336 "Walker" in 00ef0090; Alloc 00ef39c0; vtable 012f63ec slot 1; PDB ?Main@CWalker@CPersonalScript_EmptyScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); replaces CCham::Main: current name also held by 00ef84d0,00f14770; CCham::Main is at 00f14770 (its own vtable slot) (wave3/labels_alloc.tsv); PDB ?Main@CWalker@CPersonalScript_EmptyScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CCham@@QAEXXZ`; this source now defines the PDB member (class CCham renamed CWalker); same bytes.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

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
    virtual ~CIsThingAlive();                                  // 0x00E241B0 (out of line in this module)
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

namespace NScript {
class CPersonalScript_EmptyScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CWalker;
};
}
using NScript::CPersonalScript_EmptyScript;
namespace NScript {
class CPersonalScript_EmptyScript::CWalker : public CEntityScriptBase {
public:
private:
    virtual void Main();                          // PDB ?Main@CWalker@CPersonalScript_EmptyScript@NScript@@EAEXXZ
};
}

void NScript::CPersonalScript_EmptyScript::CWalker::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing inMap1 = Game->GetThingWithScriptName("InMap1");
    CScriptThing inMap2 = Game->GetThingWithScriptName("InMap2");
    Game->EntitySetAsScared(Me, true);
    CScriptGameResourceObjectScriptedThingBase scripted;
    if (scripted.IsValid())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    scripted.MoveToThing(inMap1, 0.0f, (EScriptEntityMoveType)0, true, true, false, true);
    while (!IsActiveThreadTerminating()) {
        scripted.FollowPreCalculatedRoute(inMap1, (EScriptEntityMoveType)1, true, false);
        scripted.FollowPreCalculatedRoute(inMap2, (EScriptEntityMoveType)1, true, false);
        Game->NewScriptFrame();
    }
}