#pragma optimize("s", on)
extern void __fastcall CEngineThemeGroupDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEngineThemeGroupDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CEngineThemeGroupDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}