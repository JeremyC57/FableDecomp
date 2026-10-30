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
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
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
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                             // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
}
using NScript::CIsThingAlive;
class CBaseObject { public:                               // CBaseIntelligentPointer root
    void* m_pIntelligentPointer;                   // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
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

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CFishermanWasp : public CEntityScriptBase {
public:
    void*                   __vftable;             // +0x00
    CGameScriptInterface*   Game;                  // +0x04
    CScriptThing            Me;                    // +0x08
    void Main();
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