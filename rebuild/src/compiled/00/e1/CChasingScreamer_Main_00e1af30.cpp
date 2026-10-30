// CChasingScreamer::Main() -- the chased-man screamer entity: hold an alive filter over Me, take Me under scripted control (priority 4, retried each frame), make Me fight the chased man "HKC_ChasedMan" and follow him, wait until the hero is within 4.0f, then switch Me's best enemy target to the hero and drop the scripting handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1AF30, 452 bytes; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy ctor @ 0x004ABE90 / ~CScriptThing @ 0x004AA840, CEntityScriptBase::SetPredicate @ 0x00F35B10,
//          CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, CBaseObject::CBaseObject @ 0x0099A380,
//          CScriptGameResourceObjectScriptedThingBase::IsValid @ 0x00CD23B9 / Release @ 0x00CD2770 (manifest labels
//          "C3DMeshInfo::HasPhysicsMesh" / "CMemoryDataOutputStream::Clear" are propagated; landed 0x00EB6200 names them),
//          its FollowThing(const CScriptThing&, float, bool) @ 0x007E7320 (vtable slot 7, called directly on the local) and
//          destructor @ 0x007E74D0 (labelled ~CScriptGameResourceObjectMovieBase), CCharString(const char*, long) @ 0x0099EBF0 /
//          ~CCharString @ 0x0099EAE0, IsDistanceBetweenThingsUnder @ 0x00CBE2FF (__fastcall, distance on the stack);
//          CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName
//          (by value), 451 GiveThingBestEnemyTarget (ghidra_out/cgsi_slots_aligned.tsv). Literal "HKC_ChasedMan" @ 0x012E1008.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8} as in the landed entity Mains; the scripted-thing handle (vtable
//         0x0127094C, 0x10 bytes: CBaseObject root, PImp +8/+0xC) as in the landed CSummonerToKill::Main 0x00D7DD80.
// Shape: the alive filter's destructor is inline here (its vtable kept in ebx for both stores); the handle is declared at
//        function scope after the first terminating test (own frame words); the chased man is a by-value local whose
//        destructor joins the handle's on the late exits; the name string is an implicit conversion temporary.
// Leaf: Main
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CCountedPointer.h"
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

struct CChasingScreamer : CEntityScriptBase {
    void Main();
};

void CChasingScreamer::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
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
    CScriptThing chasedMan = Game->GetThingWithScriptName("HKC_ChasedMan");
    Game->GiveThingBestEnemyTarget(Me, chasedMan);
    scripted.FollowThing(chasedMan, 1.0f, true);
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 4.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    if (scripted.IsValid())
        scripted.Release();
}