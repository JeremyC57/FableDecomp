#pragma optimize("s", on)
extern void __fastcall CExplosionDef_Destruct(void* def);
void* __fastcall CExplosionDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CExplosionDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}