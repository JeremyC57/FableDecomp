#pragma optimize("s", on)
extern void __fastcall CArmourDef_Destruct(void* def);
void* __fastcall CArmourDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CArmourDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}