// CTCGamePlayer::CTCGamePlayer(CThing* owner) -- thing-component base, the tavern game pointer cleared, the game-table intelligent pointer built
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D8FDD, length 46 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Vtables: CTCGamePlayer 0x0123EEBC (rtti-vtables.tsv), CIntelligentPointer<CThing> 0x01238C6C.
// Layout from Ego_r.pdb (CTCGamePlayer, 24 bytes): the CTCBase part +0x00..+0x0b (vptr, ParentThing, two flags),
// Game (CTCTavernGame*) +0x0c, GameTable (CIntelligentPointer<CThing>, vptr + link) +0x10.
// Shape: a size-optimised module (`push dword ptr [esp+0Ch]` pass-through, `and dword ptr [esi+0Ch],0` zero store):
// the base constructor, then the plain `Game(0)` initialiser scheduled BEFORE the derived vtable store, then the
// member intelligent pointer whose inline constructor calls the base pointer constructor and stores its own vtable.
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCGamePlayer
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("s", on)
class CThing;
class CTCTavernGame;

class CTCBase { public:                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCGamePlayer : public CTCBase { public:
    CTCTavernGame*             Game;                    // +0x0c
    CIntelligentPointer<CThing> GameTable;               // +0x10
    virtual void v00();
    CTCGamePlayer(CThing* owner);
};

CTCGamePlayer::CTCGamePlayer(CThing* owner)
    : CTCBase(owner), Game(0), GameTable()
{
}