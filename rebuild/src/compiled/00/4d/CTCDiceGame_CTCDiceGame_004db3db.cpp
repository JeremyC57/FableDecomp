// CTCDiceGame::CTCDiceGame(CThing* parent) -- CTCBase(parent), NumberOfPlayers = 0, own vtable, null CurrentPlayer pointer
// Leaf: ??0CTCDiceGame@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCDiceGame@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB3DB, length 38 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800; CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0
// (out of line, called with 0).
// Vtable 0x0124093C = CTCDiceGame (rtti-vtables.tsv). PDB CTCDiceGame (0x18 bytes): NumberOfPlayers +0x0c,
// CurrentPlayer (CIntelligentPointer<CThing>) +0x10.
// Shape: /Os (pass-through base argument, `and dword ptr [esi+0Ch],0` zero store); the scalar
// initialiser is stored before the vtable store, the pointer member's out-of-line constructor follows it.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

struct CIntelligentPointer_CThing {                         // vptr + PData
    void* __vftable;
    void* PData;
    CIntelligentPointer_CThing(CThing* thing);              // 0x004A96F0
};

class CTCDiceGame : public CTCBase { public:
    long                       NumberOfPlayers;             // +0x0c
    CIntelligentPointer_CThing CurrentPlayer;               // +0x10
    CTCDiceGame(CThing& parent);
};

CTCDiceGame::CTCDiceGame(CThing& parent)
    : CTCBase(parent)
    , NumberOfPlayers(0)
    , CurrentPlayer(0)
{
}