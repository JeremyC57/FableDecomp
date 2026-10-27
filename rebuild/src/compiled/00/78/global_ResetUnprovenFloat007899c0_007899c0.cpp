// ResetUnprovenFloat_007899c0() -- set the CTCHeroRangedAttacks module's float global 0x013BAE04 to -1.0f
// Leaf: ?ResetUnprovenFloat_007899c0@@YAXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007899c0, 11 bytes, cdecl, no arguments.
// Identity (descriptive): an external void() function at the head of the CTCHeroRangedAttacks code (0x007899C0, between GetBribeAmount 0x00789900 and SetButtonPressure 0x007899E0) called directly from Init_RegisterGameComponents 0x004EE23F (call at 0x004EE272) in its run of per-module init calls; it stores -1.0f into the .bss float 0x013BAE04 that FireRangedWeapon reads (0x0078A4B1/0x0078A4D1). Function and global names unrecovered.
// Label correction: the catalog's `$E2` is propagated by shape; a compiler dynamic initializer has internal linkage and
//   sits in .text$yc at the end of .text, while this body is in its module's code and is called directly from another TU.
extern float g_UnprovenFloat_013bae04;                         // 0x013BAE04 (.bss), read by CTCHeroRangedAttacks::FireRangedWeapon 0x0078A4B1
void ResetUnprovenFloat_007899c0()
{
    g_UnprovenFloat_013bae04 = -1.0f;
}