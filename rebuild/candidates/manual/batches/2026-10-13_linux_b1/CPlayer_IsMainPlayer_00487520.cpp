// CPlayer::IsMainPlayer()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487520, oracle length 19 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,[ecx+28h] ; mov ecx,[ecx+10h] ; call <PlayerManager->GetMainPlayerNumber> ; cmp esi,eax ; sete al ; pop esi ; ret
// engine/CPlayer.h (PDB): +0x10 CPlayerManager* PlayerManager, +0x28 long Number.
// A player is "main" when its own Number equals the number the manager reports as main.
// The callee 0x00449700 is __thiscall on the manager with no stack arguments; its manifest
// label is a propagated byte-identical name and is not trusted, so it is named from use here.
#include "engine/CPlayer.h"

// Call-surface view of CPlayerManager (engine/CPlayerManager.h carries the layout only).
struct CPlayerManagerCalls {
    long GetMainPlayerNumber();           // 0x00449700
};

struct CPlayer_Methods : CPlayer {
    bool IsMainPlayer();
};

bool CPlayer_Methods::IsMainPlayer()
{
    CPlayerManagerCalls* manager = reinterpret_cast<CPlayerManagerCalls*>(this->PlayerManager);
    return manager->GetMainPlayerNumber() == this->Number;
}
