#pragma optimize("s", on)
extern void __fastcall CObjectFamilyDef_Destruct(void* def);
void* __fastcall CObjectFamilyDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CObjectFamilyDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}