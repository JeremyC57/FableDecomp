// CKrakenMinion::Main() -- kraken minion entity script: register the alive-filter, run a frame, then (unless done) keep trying to start scripting Me at priority 3, add the "HUD_ORB_QUEST_CORE" minimap marker and give Me the hero as its best enemy target
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF4610, 281 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 (NScript::CIsThingConscious filter, vtable 0x012C2FE8), filter registration
//   0x00F35B10 / IsDone 0x00F35B30 on `this` (propagated manifest labels), CBaseObject ctor 0x0099A380 (labelled
//   CBaseIntelligentPointer), the scripted-thing handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0
//   (labels HasPhysicsMesh / Clear / ~CScriptGameResourceObjectMovieBase are propagated), CCharString 0x0099EBF0 / 0x0099EAE0,
//   CGameScriptInterface slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 350 MiniMapAddMarker,
//   451 GiveThingBestEnemyTarget.
// Shape: the landed CFH_BriarRose / CV_ChickenKickingScript models: the handle is a 0x10-byte local (out-of-line base ctor,
//   own vtable store, PImp words zeroed through eax, out-of-line dtor on every exit after its construction); the start loop is
//   `while (!Start(...)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` (rotated); the marker name is an implicit conversion.
namespace NScript { class CPredicateBase; }
class CScriptThing { public:                          // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
    virtual ~CScriptThing();                          // 0x004AA840
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGH = 3 };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CCountedPointerWords {                  // {Data, Info}
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                 // +0x08 Data, +0x0c Info
    virtual void v00();
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                        // 0x00CD23B9
    void Release();                            // 0x00CD2770
};
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
class CIsThingConscious : public CPredicateBase { public:                         // vtable 0x012C2FE8 (operator(): IsAlive() && !IsUnconscious(), CScriptThing slots 75/61)
    CScriptThing m_thing;                     // +0x04
    CIsThingConscious(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingConscious() { }
};
}
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CKrakenMinion : public CEntityScriptBase {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    void Main();
};

void CKrakenMinion::Main()
{
    {
        NScript::CIsThingConscious filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating()) {
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    }
}