// CPlayerManager::PrepareForLoad() -- tell every player to prepare for a load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449C30, length 47 bytes.
// Callees: 0x00487550 (each CPlayer's PrepareForLoad; the manifest's `_Dest_val<...>` label on it is
// propagated; Ego_r.pdb has ?PrepareForLoad@CPlayer@@QAEXXZ).
// Layout: retail std::vector is 12 bytes, so Players {first +0x0c, last +0x10} and the members after it sit 4
// bytes below Ego_r.pdb (PlayerNeutral +0x18, MainPlayer +0x1c); the engine header CPlayerManager.h carries
// the PDB offsets, so the class is modelled locally.
// Shape: `for (unsigned i = 0; i < Players.size(); ++i) Players[i]->PrepareForLoad();` -- size re-derived
// (`sub ; sar 2`) after every call, unsigned `jb`.
#include "engine/CPlayer.h"

struct CPlayer_Calls : CPlayer {
    void PrepareForLoad();                                  // 0x00487550
};

struct CPlayerVector {                                      // retail std::vector<CPlayer*>, 12 bytes
    CPlayer_Calls** m_pFirst;
    CPlayer_Calls** m_pLast;
    CPlayer_Calls** m_pEnd;
    unsigned int size() const { return m_pLast - m_pFirst; }
    CPlayer_Calls*& operator[](unsigned int i) { return m_pFirst[i]; }
};

struct CGameDefinitionManager;
struct CMainGameComponent;

struct CPlayerManager {
    void*                   __vftable;                      // +0x00
    CMainGameComponent*     Component;                      // +0x04
    CGameDefinitionManager* DefinitionManager;              // +0x08
    CPlayerVector           Players;                        // +0x0c
    long                    PlayerNeutral;                  // +0x18
    long                    MainPlayer;                     // +0x1c
    void PrepareForLoad();
};

void CPlayerManager::PrepareForLoad()
{
    for (unsigned int i = 0; i < Players.size(); ++i)
        Players[i]->PrepareForLoad();
}
