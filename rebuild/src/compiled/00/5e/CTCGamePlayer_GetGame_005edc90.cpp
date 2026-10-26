// CTCGamePlayer::GetGame() -- the tavern game this player is in, or null when the game table is gone or dead
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

class CBaseIntelligentPointer { public:                     // 8 bytes: vptr + the pointee link
    void*   m_vtable;
    void*   m_pItem;
public:
    CBaseObject* GetPItem() const;   // 0x00A01B50
};

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