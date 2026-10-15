// CTCTavernGameCoinBase::LeaveGame() -- tavern-game LeaveGame, then kills the three spawned coin things (+0xF0/+0xF4/+0xF8) and clears the pointers
// Retail: Fable.exe @ 0x008EF110, 91 bytes (vtable 0x0126F224 slot 30).
// Callees: CTCTavernGame::LeaveGame 0x005EC650 (base slot 30, called non-virtually),
//          CThing::Kill(bool) 0x004C9B80 (non-virtual member, `push 1`).
// Shape: three null-tested kills, then the pointer clears in REVERSE member order (+0xF8, +0xF4,
//        +0xF0) as immediate stores.
class CThing {
public:
    void Kill(bool immediate);          // 0x004C9B80
};

struct CTCTavernGame {
    unsigned char _pad[0xf0];
    void LeaveGame();                   // 0x005EC650
};

struct CTCTavernGameCoinBase : CTCTavernGame {
    CThing* m_pCoin0;                   // +0xf0
    CThing* m_pCoin1;                   // +0xf4
    CThing* m_pCoin2;                   // +0xf8
    void LeaveGame();
};

void CTCTavernGameCoinBase::LeaveGame()
{
    CTCTavernGame::LeaveGame();
    if (m_pCoin0)
        m_pCoin0->Kill(true);
    if (m_pCoin1)
        m_pCoin1->Kill(true);
    if (m_pCoin2)
        m_pCoin2->Kill(true);
    m_pCoin2 = 0;
    m_pCoin1 = 0;
    m_pCoin0 = 0;
}
