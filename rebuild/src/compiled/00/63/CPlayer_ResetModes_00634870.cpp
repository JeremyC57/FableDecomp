// CPlayer::ResetModes()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634870, oracle length 20 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov al,[ecx+208h] ; test al,al ; je R ; push 1 ; push 0 ; call CPlayer::AddMode ; R: ret
// engine/CPlayer.h (PDB): +0x208 bool Local. Only a local (non-remote) player gets its mode
// stack reset to mode 0 via CPlayer::AddMode(EPlayerMode, bool) @ 0x006345C0 with the flag set.
// Naming wave 16: the CPlayer_Methods stand-in respelled to CPlayer on the layout header (#define-Layout precedent 0x00830150);
//   AddMode takes the Ego_r EPlayerMode (name-only: the bytes are unchanged).
#define CPlayer CPlayer_Layout
#include "engine/CPlayer.h"
#undef CPlayer

enum EPlayerMode { PLAYER_MODE_NULL = 0 };    // enumerator name is ours
class CPlayer : public CPlayer_Layout { public:
    void AddMode(EPlayerMode mode, bool exclusive);   // 0x006345C0
    void ResetModes();
};

void CPlayer::ResetModes()
{
    if (this->Local)
        AddMode(PLAYER_MODE_NULL, true);       // EPlayerMode 0 == default/no mode
}