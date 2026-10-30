extern void* g_AutoTinyOptionalObject_013d2d1c;
// Leaf: DisplayProgress
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject_013d2d1c != 0)
        free(g_AutoTinyOptionalObject_013d2d1c);
}