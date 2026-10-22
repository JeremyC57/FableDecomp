#pragma optimize("s", on)
extern void __fastcall CTavernGameCardBaseDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTavernGameCardBaseDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CTavernGameCardBaseDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}