// CCham::Main() -- Cham's walk between the two map markers: hold an alive filter over Me, look up "InMap1" / "InMap2", make Me scared, take Me under scripted control (priority 4, retried each frame), move to InMap1, then keep following the precalculated routes to InMap1 and InMap2 every frame
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

enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CCPPointerInfo { public:                         // CCountedPointer's shared block (Ego_r.pdb CCPPointerInfo)
    long RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);         // +4 destroys the pointee
    void* Data;                                        // +8
};
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
template <class T> class CCountedPointer { public:     // retail CCountedPointer<T>: Data +0, Info +4; the release
    T*              Data;                              // dtor is 0x007E74D0's inline PImp release (wave 6 GRO rule:
    CCPPointerInfo* Info;                              // VC7.1's per-site inlining of the implicit STB dtor follows
    CCountedPointer() : Data(0), Info(0) {}            // from this size)
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

struct CCham : CEntityScriptBase {
    void Main();
};

void CCham::Main()
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