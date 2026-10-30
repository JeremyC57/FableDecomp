// CMinionEscortEnd::Main() -- the clifftop-chase escort's final stand: alive-filter over Me, take scripting control of Me, turn it to face the hero, wait until it is aware of the hero, then make the hero its target and an attention-grabbing opinion source, release control and idle until done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA47A0, length 371 bytes (padded to 384), `ret`.
// Callees: as the landed sibling CMinionEscort::Main 0x00DA45C0 (same TU): CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 in the
//   CIsThingAlive filter (vtable 0x012C32F0), filter registration 0x00F35B10, IsDone 0x00F35B30; the scripted-thing handle as the
//   landed CUndeadRisingZombieAttacker::Main 0x00E0CFF0: CBaseObject ctor 0x0099A380, IsScripting 0x00CD23B9, Release 0x00CD2770,
//   destructor 0x007E74D0 (vtable 0x0127094C); CScriptThing virtual slot 16 (+0x40) IsAwareOfHero 0x004AAAC0.
// CGameScriptInterface virtuals: NewScriptFrame 7, StartScriptingEntity 8 (priority 4), GetHero 70, GiveThingBestEnemyTarget 451,
//   EntitySetFacingAngleTowardsThing 475, OpinionSourceSetAsAttentionGrabbing 625 (+0x9c4, 0x0089C510; manifest name).
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}.
// Shape: the handle is a function-scope local destroyed once at the merged exit; every early `return` after a frame shares it;
//   the facing/target calls fetch the interface vtable before the inner GetHero (VC evaluates the callee first).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    ~CCharString();                           // 0x0099EAE0
};

class CScriptThing { public:                          // 12 bytes, polymorphic
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual bool IsAwareOfHero() const;        // slot 16 (+0x40) 0x004AAAC0
    unsigned char _body[8];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
    virtual ~CScriptThing();                          // 0x004AA840
};

struct CCountedPointerWords {                  // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                 // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsScripting();                        // 0x00CD23B9
    void Release();                            // 0x00CD2770
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };

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
class CIsThingAlive : public NScript::CPredicateBase { public:                         // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;

class CEntityScriptBase
{
public:
    virtual void v00();
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CMinionEscortEnd : public CEntityScriptBase {
public:
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    void Main();
};

void CMinionEscortEnd::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
    while (!Me.IsAwareOfHero()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    Game->OpinionSourceSetAsAttentionGrabbing(Me, true);
    handle.Release();
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}