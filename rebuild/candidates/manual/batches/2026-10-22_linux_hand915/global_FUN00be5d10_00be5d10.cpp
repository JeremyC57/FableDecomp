// _global::FUN_00be5d10(long) -- scale an integer colour channel to [0,1]: value * (1/255)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE5D10, 15 bytes, __fastcall (value in ecx): `push ecx ; mov [esp],ecx ;
//   fild dword [esp] ; fmul dword [1/255] ; pop ecx ; ret` (float constant 0x01231724 = 0x3B808081 = 1/255).
//   Unreferenced; sits among CLandscapeBackgroundPatch helpers.
float __fastcall FUN_00be5d10(long value)
{
    return value * (1.0f / 255.0f);
}
