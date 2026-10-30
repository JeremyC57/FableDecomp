// CSP_AmbushGuard::Main() -- the secret-passage ambush guard: alive filter on Me, take scripting control, wait until the hero sees or hears the guard, then have the guard shout and set the hero as his enemy
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DED540, length 470 bytes (padded to 480). Entity-script slot 1 (Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the CIsThingAlive filter, vtable
//   0x012C32F0), the filter registration @ 0x00F35B10, IsDone @ 0x00F35B30 (on `this`), CBaseObject ctor @ 0x0099A380,
//   the scripted-thing handle's IsScripting @ 0x00CD23B9 / Release @ 0x00CD2770 / destructor @ 0x007E74D0 (vtable
//   0x0127094C); CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0; CGameScriptInterface slots
//   7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 364 AddNewConversation (+0x5b0), 366 AddLineToConversation
//   (+0x5b8), 451 GiveThingBestEnemyTarget (+0x70c), 632 CanThingBe_Seen_ByOtherThing (+0x9e0), 635
//   CanThingBe_Heard_ByOtherThing (+0x9ec).
// Layout: the entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_SecretPassageScript*), PMasterData +0x18.
// Shape: the landed CUndeadRisingZombieAttacker::Main (0x00E0CFF0) prelude and scripted-thing handle; the handle is
//   released again before its destructor at the end; the line text is passed straight to the `const CCharString&`.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
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
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                   // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
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
    bool IsScripting();                                  // 0x00CD23B9
    void Release();                                      // 0x00CD2770
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_SecretPassageScript;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CSP_AmbushGuard : public CEntityScriptBase {
public:
    void*                   __vftable;                   // +0x00
    CGameScriptInterface*   Game;                        // +0x04
    CScriptThing            Me;                          // +0x08
    CQ_SecretPassageScript* ParentClass;                 // +0x14
    void*                   PMasterData;                 // +0x18
    void Main();
};

void CSP_AmbushGuard::Main()
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
    if (handle.IsScripting())
        handle.Release();
}