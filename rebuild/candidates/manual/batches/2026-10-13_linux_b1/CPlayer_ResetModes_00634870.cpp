// CPlayer::ResetModes()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634870, oracle length 20 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov al,[ecx+208h] ; test al,al ; je R ; push 1 ; push 0 ; call CPlayer::AddMode ; R: ret
// engine/CPlayer.h (PDB): +0x208 bool Local. Only a local (non-remote) player gets its mode
// stack reset to mode 0 via CPlayer::AddMode(EPlayerMode, bool) @ 0x006345C0 with the flag set.
#include "engine/CPlayer.h"

struct CPlayer_Methods : CPlayer {
    void AddMode(long mode, bool exclusive);   // 0x006345C0 (EPlayerMode, bool)
    void ResetModes();
};

void CPlayer_Methods::ResetModes()
{
    if (this->Local)
        AddMode(0, true);                      // EPlayerMode 0 == default/no mode
}
