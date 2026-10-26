#pragma optimize("s", on)
extern void __fastcall CBuyableHouseDef_Destruct(void* def);
void* __fastcall CBuyableHouseDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CBuyableHouseDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}