#pragma optimize("s", on)
extern void __fastcall CCarryingDef_Destruct(void* def);
void* __fastcall CCarryingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CCarryingDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}