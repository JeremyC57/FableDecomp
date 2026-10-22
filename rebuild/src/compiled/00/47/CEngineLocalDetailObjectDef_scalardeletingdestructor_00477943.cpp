#pragma optimize("s", on)
extern void __fastcall CEngineLocalDetailObjectDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEngineLocalDetailObjectDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CEngineLocalDetailObjectDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}