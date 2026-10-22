#pragma optimize("s", on)
extern void __fastcall CEnvironmentThemeDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEnvironmentThemeDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CEnvironmentThemeDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}