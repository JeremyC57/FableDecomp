#pragma optimize("s", on)
extern void __fastcall CObjectFamilyDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CObjectFamilyDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CObjectFamilyDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}