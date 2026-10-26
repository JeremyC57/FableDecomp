// CTCFineDialogue::IsAllowed() -- the fine dialogue may start only when the player is offered the chance to pay, the main player is free, no blocking game-UI state is up and the world's mode is 0 or 9
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2790, length 83 bytes.
// Callees: the CTCBase accessor @ 0x00686D60 (`[g_pMainGameComponent 0x013B8A1C]+0x1c`; object not identified, modelled by the
// record at its +0x28 -- landed 0x007E1F70 calls it GetSubsystem)
// and CTCBase::GetWorld() @ 0x00686A70 (`+0x24`, PWorld; landed naming of 0x006588DC); the game UI singleton @ 0x013B8790.
// Layout: PDB CTCFineDialogue (28 bytes) over CTCBase (engine/CTCBase.h): Fine +0xc, Money +0x10,
// GivePlayerChanceToPayFine +0x14, WaitingForConfirmation +0x15, Village +0x18. The other records are opaque here:
// the player manager's main-player record at +0x28 (busy flag byte +0xbc), two game-UI flag bytes (+0xfd, +0xf6),
// the world's mode word at +0x104.
// Shape: `this` in esi (ecx still holds it for the first getter, `mov ecx,esi` before the second); every early
// `return false` shares the `xor al,al` exit; the closing `if (mode == 0 || mode == 9) return true; return false;`
// keeps byte exits (`mov al,1`) where `return a || b;` would materialise a full register; the UI pointer is read once.
#include "engine/CTCBase.h"

struct CPlayerRecord_007e2790 {
    unsigned char _pad[0xbc];
    bool          m_busy;                          // +0xbc
};

struct CPlayerManager_007e2790 {
    unsigned char           _pad[0x28];
    CPlayerRecord_007e2790* m_pMainPlayer;         // +0x28
};

struct CGameUI_007e2790 {                          // the game UI singleton behind the pointer at 0x013B8790
    unsigned char _pad00[0xf6];
    bool          m_blockingF6;                    // +0xf6
    unsigned char _padF7[0x6];
    bool          m_blockingFD;                    // +0xfd
};
extern CGameUI_007e2790* g_pGameUI_013b8790;       // 0x013B8790

struct CWorld_007e2790 {
    unsigned char _pad[0x104];
    long          m_mode;                          // +0x104
};

class CTCFineDialogue : public CTCBase { public:
    long  Fine;                                    // +0x0c
    long  Money;                                   // +0x10
    bool  GivePlayerChanceToPayFine;               // +0x14
    bool  WaitingForConfirmation;                  // +0x15
    void* Village;                                 // +0x18 CTCVillage*
    CPlayerManager_007e2790* GetGameSubsystem1C(); // 0x00686D60
    CWorld_007e2790* GetWorld();                   // 0x00686A70
    bool IsAllowed();
};

bool CTCFineDialogue::IsAllowed()
{
    if (!GivePlayerChanceToPayFine)
        return false;
    if (GetGameSubsystem1C()->m_pMainPlayer->m_busy)
        return false;
    if (g_pGameUI_013b8790->m_blockingFD)
        return false;
    if (g_pGameUI_013b8790->m_blockingF6)
        return false;
    long mode = GetWorld()->m_mode;
    if (mode == 0 || mode == 9)
        return true;
    return false;
}