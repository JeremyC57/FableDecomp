// CPlayer::IsMainPlayer()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487520, oracle length 19 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,[ecx+28h] ; mov ecx,[ecx+10h] ; call <PlayerManager->GetMainPlayerNumber> ; cmp esi,eax ; sete al ; pop esi ; ret
// engine/CPlayer.h (PDB): +0x10 CPlayerManager* PlayerManager, +0x28 long Number.
// A player is "main" when its own Number equals the number the manager reports as main.
// The callee 0x00449700 is __fastcall on the manager with no stack arguments; its manifest
// label is a propagated byte-identical name and is not trusted, so it is named from use here.
//
// Lane feedback: Number must be read into a local before the call (retail keeps it in esi across the call),
// and the bool result needs the `?:` form for a direct `sete al`. RELOCATION_MATCH.
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
    // Retail reads this->Number into esi BEFORE the manager call: the source read it into a
    // local first (a bare `call() == this->Number` lets the compiler load it after the call).
    long number = this->Number;
    CPlayerManagerCalls* manager = reinterpret_cast<CPlayerManagerCalls*>(this->PlayerManager);
    return (number == manager->GetMainPlayerNumber()) ? true : false;
}