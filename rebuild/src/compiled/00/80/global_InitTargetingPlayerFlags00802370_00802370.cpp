// InitTargetingPlayerFlags_00802370() -- set the CTCTargetingPlayer module's two flag globals to their start values (first on, second off)
// Leaf: ?InitTargetingPlayerFlags_00802370@@YAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00802370, 15 bytes, cdecl, no arguments.
// Identity (descriptive): an external void() function at the head of the CTCTargetingPlayer code (before the landed
//   CTCTargetingPlayer::SetAsPickUpNewLockTarget 0x008023A0), called directly from Init_RegisterGameComponents 0x004EE23F
//   (call at 0x004EE28B) in its run of per-module init calls, next to the landed ResetUnprovenFloat_007899c0 and
//   ResetActionSpeedMultiplierOverride_007fffa0. It stores 1 into the .bss byte 0x013BAF20 (tested at 0x0080517F in the
//   targeting code) and 0 into 0x013BAF21 (tested at 0x00804234). No PDB names either global; names are descriptive.
// Label correction: the catalog's `$E2` is propagated by shape; a compiler dynamic initializer has internal linkage and sits
//   in .text$yc at the end of .text, while this body is in its module's code and is called directly from another TU.
// Shape: two immediate byte stores (`mov byte ptr [g],1` / `[g],0`).
extern bool g_TargetingFlag_013baf20;           // 0x013BAF20 (.bss), read at 0x0080517F
extern bool g_TargetingFlag_013baf21;           // 0x013BAF21 (.bss), read at 0x00804234

void InitTargetingPlayerFlags_00802370()
{
    g_TargetingFlag_013baf20 = true;
    g_TargetingFlag_013baf21 = false;
}