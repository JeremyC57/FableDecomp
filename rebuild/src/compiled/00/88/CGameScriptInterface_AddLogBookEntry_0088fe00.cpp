// CGameScriptInterface::AddLogBookEntry(long a, long b, long c, long d) const -- add an entry to the hero's log book (the player from the manager at +0x14 via 0x00449970, its log book at +0xf8, 0x0047A9F0 with the four arguments and 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088FE00, length 48 bytes.
// Callees: the player getter @ 0x00449970 (labelled "Catch@0040d959", propagated), CHeroLogBook::AddLogEntry @ 0x0047A9F0 (labelled "CHeroLogBook_AddLogEntry"). Shape: the four arguments re-read as the pushes shift the stack, `ret 10h`.
struct CDisplayEngine_Retail;
class CPlayerManager;
class CWorld;
struct CHeroLogBook_Retail {
    void AddLogEntry(long a, long b, long c, long d, long flag);   // 0x0047A9F0
};
class CPlayer { public:
    unsigned char       _pad[0xf8];
    CHeroLogBook_Retail* m_pLogBook;               // +0xf8
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                 World;                           // +0x04 (engine/CGameScriptInterface.h)
    long                    Component;
    CDisplayEngine_Retail*  DisplayEngine;                   // +0x0c
    long                    DefinitionManager;
    CPlayerManager*  PlayerManager;                   // +0x14
    unsigned char           _pad18[0x15];
    bool                    AllowScreenFadingIfAlreadyFaded; // +0x2d
    void AddLogBookEntry(long a, long b, long c, long d) const;
};

void CGameScriptInterface::AddLogBookEntry(long a, long b, long c, long d) const
{
    CPlayer* player = &PlayerManager->GetMainPlayer();
    player->m_pLogBook->AddLogEntry(a, b, c, d, 1);
}