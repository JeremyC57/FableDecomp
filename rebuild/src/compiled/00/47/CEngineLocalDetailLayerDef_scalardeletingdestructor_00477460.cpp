#pragma optimize("s", on)
extern void __fastcall CEngineLocalDetailLayerDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEngineLocalDetailLayerDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CEngineLocalDetailLayerDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}