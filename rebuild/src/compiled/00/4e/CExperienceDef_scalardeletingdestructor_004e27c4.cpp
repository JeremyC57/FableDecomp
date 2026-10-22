#pragma optimize("s", on)
extern void __fastcall CExperienceDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CExperienceDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CExperienceDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}