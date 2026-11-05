// CTCGamePlayer::GetGame() -- the tavern game this player is in, or null when the game table is gone or dead
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EDC90, length 45 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on GameTable).
// Layout from Ego_r.pdb (CTCGamePlayer, 24 bytes): Game (CTCTavernGame*) +0x0c, GameTable (CIntelligentPointer<CBaseObject>) +0x10;
// retail CBaseObject: the Dead bit = bit 0 of +0x91.
// Shape: the pointer's address held in esi for both GetPItem calls (the item is fetched again for the Dead test),
// nested `if`s sharing the `xor eax,eax` tail.
class CTCTavernGame;

class CBaseObject { public:
    unsigned char _pad_0x00[0x91];
    unsigned char Dead : 1;                             // +0x91 bit 0 (retail)
    unsigned char _flags_0x91 : 7;
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCGamePlayer { public:
    unsigned char              _pad_0x00[0x0c];         // CTCBase: vptr, ParentThing, flags
    CTCTavernGame*             Game;                    // +0x0c
    CBaseIntelligentPointer GameTable;               // +0x10
    CTCTavernGame* GetGame();
};

CTCTavernGame* CTCGamePlayer::GetGame()
{
    if (GameTable.GetPItem()) {
        if (!GameTable.GetPItem()->Dead)
            return Game;
    }
    return 0;
}