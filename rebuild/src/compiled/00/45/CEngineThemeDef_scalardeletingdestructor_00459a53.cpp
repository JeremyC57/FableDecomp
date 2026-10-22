#pragma optimize("s", on)
extern void __fastcall CEngineThemeDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEngineThemeDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CEngineThemeDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}