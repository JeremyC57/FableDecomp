extern "C" int __cdecl rand();                 // 0x00BFEB16 MSVCR71 rand (import thunk)

int __fastcall NBalverineCombat_SetLungedThisFrame(int n)
{
    return rand() % n;
}