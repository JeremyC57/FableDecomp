// NScript::CTonyTestScriptsScript::CDummy::Main() -- entity-script Main (vtable 0x012F701C slot 1): register an is-alive functor over Me, run one script frame, then (unless done) flag Me as having information and keep trying, a frame at a time, to start scripting Me through a fresh local handle
// Identity (naming wave 11): ?Main@CDummy@CTonyTestScriptsScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F701C slot 1 is stored by the factory 0x00EFF3F0 registered as "Dummy"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFF4D0, 226 bytes (+ int3 padding).
// Identity: the catalog/BSim label "NScript::CQ_EndGameFocalSitesScript::CEndGameScarletRobe::Main" is a 0.58-similarity guess;
//   the body is slot 1 of vtable 0x012F701C, built by the 0x1c-byte factory 0x00EFF3F0 (registered under the name "Dummy"
//   at 0x00EFF14E). 28 bytes matches the PDB CEndGameScarletRobe, so the label is kept; the true class is unproven.
// Callees: CScriptThing copy ctor 0x004ABE90 (the functor's member), the functor registration 0x00F35B10 and IsDone 0x00F35B30
//   (on `this`), the functor's out-of-line dtor 0x00EF7AB0 (vtable 0x012C2FE8, landed as CScriptThingFunctor_00ef7ab0),
//   CBaseObject::CBaseObject() 0x0099A380, the handle's IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0
//   (vtable 0x0127094C = CScriptGameResourceObjectScriptedThingBase), game interface slots 7 NewScriptFrame, 8
//   StartScriptingEntity, 360 SetThingHasInformation(thing, false, true, false).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18.
// Shape: the functor is a scoped frame object (its own {} block); the handle is a function-scope local default-constructed
//   inline (CBaseObject ctor, own vtable, PImp {0,0} as an embedded member object so the zero stores follow the vtable store);
//   the `while (!Start(...)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; } IsActiveThreadTerminating();` loop is rotated with &Me in edi.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CScriptThingFunctor_00ef7ab0 : public NScript::CPredicateBase {                  // vtable 0x012C2FE8
    CScriptThing m_thing;                              // +0x04
    CScriptThingFunctor_00ef7ab0(const CScriptThing& thing) : m_thing(thing) { }
    ~CScriptThingFunctor_00ef7ab0();                   // 0x00EF7AB0
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

struct CCountedPointer_ScriptedThing {                 // PImp {Data, Info}
    void* Data;
    void* Info;
    CCountedPointer_ScriptedThing() : Data(0), Info(0) { }
};

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer_ScriptedThing PImp;                // +0x08
    CScriptGameResourceObjectScriptedThingBase() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();     // 0x007E74D0
    bool IsValid() const;                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CTonyTestScriptsScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CDummy;                        // the entity class below (naming wave 11)
};
}
using NScript::CTonyTestScriptsScript;
namespace NScript {
class CTonyTestScriptsScript::CDummy : public CEntityScriptBase {
public:
        void*                 ParentClass;                 // +0x14
    void*                 PMasterData;                 // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CTonyTestScriptsScript::CDummy::Main()
{
    SetPredicate(CScriptThingFunctor_00ef7ab0(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingHasInformation(Me, false, true, false);
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    IsActiveThreadTerminating();
}