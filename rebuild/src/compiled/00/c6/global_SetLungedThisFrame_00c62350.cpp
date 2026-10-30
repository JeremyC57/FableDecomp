extern "C" __declspec(dllimport) int __cdecl rand();   // MSVCR71 rand through its IAT slot __imp__rand 0x01440124
extern float g_scale_0129c81c;
float SetLungedThisFrame()
{
    return (float)rand() * g_scale_0129c81c;
}