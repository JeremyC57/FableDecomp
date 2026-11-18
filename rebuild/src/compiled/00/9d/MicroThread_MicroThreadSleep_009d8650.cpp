// MicroThread::MicroThreadSleep() -- the micro-thread stack switch: park the running side's registers and SEH chain head on its own stack, swap esp with MicroThread::PGlobalStackPointer, resume the side parked there
// Leaf: ?MicroThreadSleep@MicroThread@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8650, 36 bytes, __fastcall, no arguments, no compiler prologue (its pushes ARE
//   the saved frame of the side that sleeps).
// Identity (three sources): Ego_d.pdb lib_microthread.obj order DeleteMicroThread / MicroThreadSleep / MicroThreadReturn /
//   SwitchToMicroThread (0x009D87F0); FableWin.pdb ?MicroThreadSleep@MicroThread@@YIXXZ 0x02F6CA00 and Ego_r.pdb 0x00B2B980, each
//   followed by MicroThreadSleep(int) and MicroThreadReturn = retail 0x009D8680 / 0x009D8690; reloc_bindings (caller 0x00A44690).
// Global 0x013CA898 = PGlobalStackPointer (Ego_d.pdb + Anniversary PDB S_LDATA32; see engine/MicroThreadSleep.h).
// Retail-asm: 0x009D865D `mov esp, dword ptr [0x013CA898]` after 0x009D865B `mov eax, esp` (the stack switch), 0x009D8654
//   `mov eax, dword ptr fs:[0]` / 0x009D865A `push eax` and 0x009D8668 `pop eax` / 0x009D8669 `mov dword ptr fs:[0], eax` (the
//   chain head swap), 0x009D8651 `push ebp` / 0x009D8671 `pop ebp`: no C++ or VC7.1 intrinsic writes esp; the original defined
//   the whole function in assembler (Ego_d.pdb /Od length 36 = retail). The definition comes from the shared helper header
//   engine/MicroThreadSleep.h (asm path + FABLE_PORTABLE twin); the rule-4 attempt table (32 C++ + 12 intrinsic spellings, all
//   DIFFER) is in the asm1 hand-back README.
#include "engine/MicroThreadSleep.h"

namespace MicroThread
{
    FABLE_MICROTHREAD_SLEEP_DEFINITION
}