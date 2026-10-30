// Levers: inline CScriptThing model (CBaseClass + CCountedPointer, __forceinline dtor); the CAnd filter comes from an inline by-value `And(a, b)` factory (the argument address is re-taken, `lea eax,[esp+48h]`); in-body `#pragma inline_depth(0)` from after the scripted-object declaration to the end keeps every later ~CScriptThing out of line (merged with the scripted-object dtor tail), leaving the single inline expansion at the early terminate return.
// CCham::Main() -- Cham's second entity script: filter on Me and the "Leader" marker, take Me under scripted control (priority 4), then run the scripted movement towards the leader until it finishes
// Leaf: Main
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);         // destroys the pointee
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
class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    __forceinline ~CScriptThing() {}                   // out of line 0x004AA840
};

enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void MoveToThing(const CScriptThing& thing, float distance, EScriptEntityMoveType moveType, bool a, bool b, bool c, bool d);   // slot 5 (+0x14)
    virtual void FollowPreCalculatedRoute(const CScriptThing& thing, EScriptEntityMoveType moveType, bool a, bool b);   // slot 6 (+0x18)
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
public:
    virtual bool IsPerformingScriptTask() const;   // 0x007E7450
    virtual void FollowThing(const CScriptThing&, float, bool);   // 0x007E7320
};

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
struct CFilterBase : public NScript::CPredicateBase {                                   // vtable 0x012C2FF4
    ~CFilterBase() { }
};
struct CIsThing {                                      // vtable 0x012C2FE8
    virtual void v00();
    CScriptThing m_thing;                              // +0x04
    CIsThing(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThing();                                       // 0x00EF7AB0
};
struct CAnd : CFilterBase {                            // vtable 0x012F687C, 0x24 bytes
    CIsThing m_a;                                      // +0x04
    CIsThing m_b;                                      // +0x14
    CAnd(const CIsThing& a, const CIsThing& b);        // 0x00EF8740
    ~CAnd() { }
    virtual void v00();
};

class CEntityScriptBase { public:
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

inline CAnd And(const CIsThing& a, const CIsThing& b) { return CAnd(a, b); }

void CCham::Main()
{
    CScriptThing leader = Game->GetThingWithScriptName("Leader");
    SetPredicate(And(CIsThing(Me), CIsThing(leader)));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase scripted;
#pragma inline_depth(0)
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    scripted.FollowThing(leader, 1.0f, true);
    while (scripted.IsPerformingScriptTask()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
#pragma inline_depth()