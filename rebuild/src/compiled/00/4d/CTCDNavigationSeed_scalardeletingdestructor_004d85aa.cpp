#pragma optimize("s", on)
extern void __fastcall CTCDNavigationSeed_Destruct(void* seed);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDNavigationSeed_ScalarDeletingDestructor(void* seed, int, unsigned int flags)
{
    CTCDNavigationSeed_Destruct(seed);
    if (flags & 1)
        OperatorDelete(seed);
    return seed;
}