#pragma optimize("s", on)
extern void __fastcall CExplosionDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CExplosionDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CExplosionDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}