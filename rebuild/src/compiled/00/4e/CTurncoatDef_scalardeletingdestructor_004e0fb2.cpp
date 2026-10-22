#pragma optimize("s", on)
extern void __fastcall CTurncoatDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTurncoatDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CTurncoatDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}