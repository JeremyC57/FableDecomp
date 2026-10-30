// CFishermanWasp::Main() -- the wasp-quest fisherman entity script: register an alive-filter over Me, start a frame, and
// unless the script is done set the hero as Me's best enemy target and keep running frames until it is done
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7BB30, length 162 bytes (padded to 176), `ret`.
// Callees: CScriptThing copy ctor @ 0x004ABE90, the filter registration @ 0x00F35B10 (on `this`), ~CIsThingAlive
// @ 0x00E241B0 (out of line in this module), IsDone @ 0x00F35B30, CBaseObject::CBaseObject @ 0x0099A380 (the
// scripted-thing handle's root), ~CScriptGameResourceObjectScriptedThingBase @ 0x007E74D0 (labelled
// ~CScriptGameResourceObjectMovieBase), CGameScriptInterface slots 7 NewScriptFrame (+0x1C), 70 GetHero (+0x118) and
// 451 GiveThingBestEnemyTarget (+0x70C) (ghidra_out/cgsi_slots_aligned.tsv). Vtables: CIsThingAlive 0x012C32F0,
// CScriptGameResourceObjectScriptedThingBase 0x0127094C.
// Layout (the CEntityScriptBase prefix of the landed entity scripts): vptr, Game +4, CScriptThing Me +8.
// Shape: the landed CSickChildFishingSpot/CHHUndead::Main prelude (block-scoped filter); the handle is a function-scope
// local after an early `if (IsActiveThreadTerminating()) return;` (a block-scoped handle shares the filter's frame words, retail keeps
// a 0x20-byte frame); a scripted-thing handle local (never used) built after
// the done test, so its out-of-line dtor runs after the `do { NewScriptFrame(); } while (!IsActiveThreadTerminating());` loop;
// `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` fetches the outer vtable before the inner call.

namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                     // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual void v00();
    virtual ~CScriptGameResourceObjectScriptedThingBase(); // 0x007E74D0
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CFishermanWasp : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CFishermanWasp::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}