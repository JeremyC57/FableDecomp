// CUndeadRisingZombieAttacker::Main() -- conscious filter on Me, take scripting control of the zombie, aim it at the hero, wait for the parent quest to reach state 2, then remove it
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0CFF0, length 312 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the CIsThingConscious filter, vtable
// 0x012C2FE8), the filter registration @ 0x00F35B10, IsActiveThreadTerminating @ 0x00F35B30, CBaseIntelligentPointer /
// CBaseObject ctor @ 0x0099A380, the scripted-thing handle's IsValid @ 0x00CD23B9 / Release @ 0x00CD2770 / destructor
// @ 0x007E74D0 (vtable 0x0127094C), CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero,
// 451 GiveThingBestEnemyTarget (+0x70c), 108 RemoveThing (+0x1b0).
// Layout: the entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14 (the undead-rising quest script, its state at
// +0x5c); CScriptGameResourceObjectScriptedThingBase as the landed CV_ChickenKickingScript ctor 0x00E62830 (16 bytes).
// Shape: the scoped filter object (both vtable stores through ebx), then the handle as a local destroyed out of line on
// every exit after it; `while (!StartScriptingEntity(...)) { frame; if (done) return; }`; the target call reads the
// interface's vtable before GetHero(); the state wait compares against 2 held in ebx.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                                    // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingConscious : public NScript::CPredicateBase {                               // vtable 0x012C2FE8: a filter holding a script thing
    CScriptThing m_thing;                                // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
struct CCountedPointerWords {                            // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                           // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                                  // 0x00CD23B9
    void Release();                                      // 0x00CD2770
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_3 = 3 };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CQ_UndeadRisingScript {
public:
    unsigned char _pad00[0x5c];
    long          State;                                 // +0x5c
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CUndeadRisingZombieAttacker : public CEntityScriptBase {
public:
    void*                  __vftable;                    // +0x00
    CGameScriptInterface*  Game;                         // +0x04
    CScriptThing           Me;                           // +0x08
    CQ_UndeadRisingScript* ParentClass;                  // +0x14
    void Main();
};

void CUndeadRisingZombieAttacker::Main()
{
    {
        CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (ParentClass->State != 2) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, true, true);
}