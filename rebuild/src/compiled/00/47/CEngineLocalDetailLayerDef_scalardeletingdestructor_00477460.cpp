#pragma optimize("s", on)
extern void __fastcall CEngineLocalDetailLayerDef_Destruct(void* def);
void* __fastcall CEngineLocalDetailLayerDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CEngineLocalDetailLayerDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}