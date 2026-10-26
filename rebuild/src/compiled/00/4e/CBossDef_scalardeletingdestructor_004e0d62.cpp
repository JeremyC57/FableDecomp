#pragma optimize("s", on)
extern void __fastcall CBossDef_Destruct(void* def);
void* __fastcall CBossDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CBossDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}