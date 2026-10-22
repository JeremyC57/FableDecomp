// _global::FUN_009be380(long, long) -- the ratio of the second argument over the first as a float
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BE380, 11 bytes: `fild dword [esp+8] ; fidiv dword [esp+4] ; ret 8`; ecx
//   is never read. Unreferenced; sits between CDisplayManager::GetRefreshRate and DestroyDevice, so it reads as
//   a rational-number (numerator/denominator) helper of the display manager. Modelled as a member that ignores this.
struct CDisplayManager_009be380 {
    float FUN_009be380(long denominator, long numerator);
};

float CDisplayManager_009be380::FUN_009be380(long denominator, long numerator)
{
    return (float)numerator / denominator;
}