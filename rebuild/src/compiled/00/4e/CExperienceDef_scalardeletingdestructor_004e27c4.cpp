#pragma optimize("s", on)
extern void __fastcall CExperienceDef_Destruct(void* def);
void* __fastcall CExperienceDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CExperienceDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}