// CTCGamePlayer::SetGame(CTCTavernGame*) -- remember the tavern game and point GameTable at the game's table thing (or at nothing when the game is cleared)
// Leaf: SetGame
//
// Retail 0x005EDC00, 141 bytes (`ret 4`). Ego_r.pdb `?SetGame@CTCGamePlayer@@QAEXPAVCTCTavernGame@@@Z`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the
// CIntelligentPointer<CThing> vtable 0x01238C6C), ::SetPItem @ 0x00A01B90 (twice per arm), ::GetPItem @
// 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10.
// Layout = Ego_r.pdb CTCGamePlayer (0x18): Game +0xc, GameTable +0x10; the tavern game's ParentThing
// (CTCBase +0x04) is its table.
// Shape: the store comes first, then each arm assigns its own CIntelligentPointer temporary (built in
// the frame from the table or from 0, inline operator= taking GetPItem() into a local before SetPItem,
// destroyed out of line) and keeps its own `ret 4`; the temporary's address is formed once above the
// branch.
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CBaseObject* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

class CTCTavernGame { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
};

class CTCGamePlayer { public:
    virtual void v00();
    CThing*        ParentThing;                             // +0x04 (CTCBase)
    bool           ToBeRemoved;                             // +0x08
    bool           IsInGlobalUpdateTCs;                     // +0x09
    CTCTavernGame* Game;                                    // +0x0c
    CIntelligentPointer<CThing> GameTable;                  // +0x10
    void SetGame(CTCTavernGame* game);
};

void CTCGamePlayer::SetGame(CTCTavernGame* game)
{
    Game = game;
    if (game)
        GameTable = CIntelligentPointer<CThing>(game->ParentThing);
    else
        GameTable = CIntelligentPointer<CThing>(0);
}