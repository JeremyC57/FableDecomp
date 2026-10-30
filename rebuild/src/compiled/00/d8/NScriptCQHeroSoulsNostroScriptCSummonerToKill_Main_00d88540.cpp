// NScript::CQ_HeroSoulsNostroScript::CSummonerToKill::Main() -- the summoner-to-kill entity script: register an alive filter over Me, update once, wait (one frame at a time) until the parent quest's KickedOffQuestStartScreen flag is set, then unless the script is done remove Me from the world (RemoveThing(Me, false, true))
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D88540, 148 bytes (vtable slot 1 = Main; PDB ?Main@CSummonerToKill@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ;
//          the body sits beside 0x00D88260 CQ_HeroSoulsNostroScript::WatchForNostroHit).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10 (on `this`), ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30, the game interface's slots 7 (+0x1c, frame
//          update) and 108 (+0x1b0, CGameScriptInterface::RemoveThing(const CScriptThing&, bool, bool) per rtti-vtables 0x008910D0).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; the parent's KickedOffQuestStartScreen is retail +0x4d
//          (landed CQ_HeroSoulsNostroScript rows: PDB offsets -0x14).
// Shape: the landed CHTE_Prisoner::Main (0x00D69F50) model: a scoped filter object, then a frame loop that bails out when the
//          script is done.
// Leaf: Main
class CScriptThing { public:                              // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    virtual ~CScriptThing();                               // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CIsThingAlive { public:                              // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
namespace NScript {
class CQ_HeroSoulsNostroScript { public:
    unsigned char _body[0x4d];
    bool          KickedOffQuestStartScreen;     // +0x4d
    class CSummonerToKill;
};
}
class NScript::CQ_HeroSoulsNostroScript::CSummonerToKill {
public:
    void*                             __vftable;   // +0x00
    CGameScriptInterface*             Game;        // +0x04
    CScriptThing                      Me;          // +0x08
    NScript::CQ_HeroSoulsNostroScript*  ParentClass; // +0x14
    void*                             PMasterData; // +0x18
    void RegisterFilter(CIsThingAlive& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void NScript::CQ_HeroSoulsNostroScript::CSummonerToKill::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!ParentClass->KickedOffQuestStartScreen) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->RemoveThing(Me, false, true);
}