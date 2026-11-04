// CTCTavernGame::SetStateAfterInstructions'::`5'::`dynamic atexit destructor for 'no_text_entry''() -- destroy the function-local static CWideString no_text_entry at exit
// Leaf: _$E2
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229150, 10 bytes: `mov ecx, offset 0x013B8C50 ; jmp 0x0099B510` (~CWideString on the static).
// Identity (two sources): (1) the only reference to 0x01229150 is the `push offset 0x01229150 ; call atexit 0x004012BC` at 0x005EB1FD
//   inside CTCTavernGame::SetStateAfterInstructions 0x005EB180, right after the guarded construction of the static at 0x013B8C50
//   (guard 0x013B8C54 bit 0, L"Missing text entry" 0x01255FE8); (2) both PDB name sets list
//   `CTCTavernGame::SetStateAfterInstructions'::`5'::`dynamic atexit destructor for 'no_text_entry'' (donors 0x0400F380 / 0x0101BF30):
//   scope 5 = a function-scope static declared after one if-block (VC7.1 numbering, probed: `if (a) { ...; return; }` then the
//   static gives ?4?), which is retail's shape (the early `LeaveGame(); return;` block at 0x005EB191..AC precedes the static).
//   Static: ?no_text_entry@?4??SetStateAfterInstructions@CTCTavernGame@@IAEXKVCWideString@@W4ETavernGameStates@2@@Z@4V3@A (the nested name shifts the backreference).
//   VC7.1 emits the stub as the TU-local `_$E2` in .text$yd, past the code (precedent: landed 0x01229170).
// TU: ABRIDGED owner. Only the thunk is graded; the owner body below models the static's declaration context (one early-exit
//   if-block, then the static) and nothing else. The real 544-byte owner 0x005EB180 (PlaySoundW, the instruction text loop) is its
//   own unworked row and must replace this body when it is authored.
// Callees: ??1CWideString@@QAE@XZ 0x0099B510.
// Header: engine/CTCTavernGame.h (FABLE_RETAIL_CTCTAVERNGAME).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
void CTCTavernGame::SetStateAfterInstructions(unsigned long, CWideString, ETavernGameStates state)
{
    if (GameFinished) {
        LeaveGame();
        return;
    }
    static CWideString no_text_entry(L"Missing text entry");
    SetState(state);
}