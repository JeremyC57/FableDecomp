#pragma optimize("s", on)
extern void __fastcall CArmourDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CArmourDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CArmourDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}