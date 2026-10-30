// NScript::CBenTestScriptsScript::CDummy::Main() -- the Scarlet Robe entity script in the end game: watch that she stays alive, make her carry no information, then keep trying (a script frame at a time) to start scripting her through a local scripted-thing handle
// Leaf: Main
// Identity (naming wave 11): ?Main@CDummy@CBenTestScriptsScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F6F14 slot 1 is stored by the factory 0x00EFE620 registered as "Dummy"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFE700, length 226 bytes (+ int3 padding), `ret`.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the alive filter's member; filter vtable
// 0x012C2FE8, its destructor called out of line @ 0x00EF7AB0 in this module; named CIsThingAliveEndGame to keep it apart from
// the CIsThingAlive copy at 0x00E241B0), the filter registration @ 0x00F35B10 and
// IsDone @ 0x00F35B30 on `this` (labels propagated; see the landed CHTE_Prisoner::Main 0x00D69F50), CBaseObject::CBaseObject
// @ 0x0099A380, CScriptGameResourceObjectScriptedThingBase (vtable 0x0127094C): IsValid @ 0x00CD23B9, Release
// @ 0x00CD2770 (labels propagated; the landed CFH_BriarRose::Main 0x00D37330 names them) and its destructor @ 0x007E74D0
// (catalog "~CScriptGameResourceObjectMovieBase"); CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 8
// StartScriptingEntity (+0x20) and 360 SetThingHasInformation (+0x5a0, thing + three bools).
// Layout = Ego_r.pdb CEndGameScarletRobe (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass
// (CBenTestScriptsScript*) +0x14, PMasterData +0x18. The handle (PDB 16 bytes): CBaseObject {vptr, +4}, PImp
// (CCountedPointer) +8.
// Shape: the filter is a scoped frame object (its out-of-line destructor); the handle is a function-scope local built
// inline (base ctor call, vtable, PImp zeroed by its own inline ctor) and destroyed out of line on both exits; the start
// loop is BriarRose's rotated `while (!Start...) { yield; if (IsActiveThreadTerminating()) return; }` followed by a plain IsActiveThreadTerminating() call.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
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
struct CIsThingAliveEndGame : public NScript::CPredicateBase {                      // vtable 0x012C2FE8: the end-game module's alive filter (its own vtable and
                                                   // out-of-line dtor; the landed 0x00E95E80 binds ~CIsThingAlive to 0x00E241B0)
    CScriptThing m_thing;                          // +0x04
    CIsThingAliveEndGame(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAliveEndGame();                       // 0x00EF7AB0
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct SCountedImp {
    void* Data;
    void* Info;
    SCountedImp() : Data(0), Info(0) { }
};
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
    SCountedImp PImp;                              // +0x08
    CScriptGameResourceObjectScriptedThingBase() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
    bool IsValid() const;                            // 0x00CD23B9
    void Release();                                // 0x00CD2770
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CBenTestScriptsScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CDummy;                        // the entity class below (naming wave 11)
};
}
using NScript::CBenTestScriptsScript;
struct CQ_SunnyvaleMasterData;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CBenTestScriptsScript::CDummy : public CEntityScriptBase {
public:
        CBenTestScriptsScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;       // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CBenTestScriptsScript::CDummy::Main()
{
    {
        CIsThingAliveEndGame filter(Me);
        SetPredicate(filter);
    }
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