// CThing::PeekPlayer() -- the player this thing belongs to: the thing manager's player manager, the const overload of
//   CPlayerManager::GetPlayer(long) (0x00449910), indexed by PlayerNumber
// Leaf: ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7A10, 25 bytes: mov eax,ecx ; movsx edx,byte [eax+90h] ; mov ecx,[0x013B8A1C] ;
//   mov ecx,[ecx+30h] ; push edx ; call 0x00449910 ; ret.
// Identity (sweep3; catalog had both 0x004C7A10 and 0x004C7A30 as CThing::GetPlayer, the sources were free __fastcall
//   stand-ins): Ego_r.pdb and FableWin.pdb continue the CThing accessor run (0x004C7990 .. 0x004C7A00, RENAMES) with
//   ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ then ?GetPlayer@CThing@@QAEAAVCPlayer@@XZ (both 0x20 apart, as 0x004C7A10 /
//   0x004C7A30 are); the first calls the const CPlayerManager::GetPlayer 0x00449910, the second the non-const 0x004498C0
//   (PDB order GetPlayer QAEAAV, GetPlayer QBEABV = retail 0x004498C0, 0x00449910). PlayerNumber +0x90 is followed by
//   the Dead bit at +0x91, as in the Ego_r CThing layout (PlayerNumber schar, then the Dead bitfield).
class CPlayer;

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CThingManager {                                // only the field read here (PC offset)
public:
    unsigned char   _pad00[0x30];
    CPlayerManager* PlayerManager;                   // +0x30
};

extern CThingManager* g_pThingManager;               // 0x013B8A1C

class CThing {
public:
    unsigned char _pad00[0x90];
    signed char   PlayerNumber;                      // +0x90 (Ego_r CThing::PlayerNumber, schar)
    const CPlayer& PeekPlayer() const;
    CPlayer&       GetPlayer();
protected:
    // 0x004C79F0 / 0x004C7A00; in this TU (thing.cpp) their bodies are visible and inlined
    const CPlayerManager& PeekPlayerManager() const { return *g_pThingManager->PlayerManager; }
    CPlayerManager&       GetPlayerManager() { return *g_pThingManager->PlayerManager; }
};

const CPlayer& CThing::PeekPlayer() const
{
    return PeekPlayerManager().GetPlayer(PlayerNumber);
}
