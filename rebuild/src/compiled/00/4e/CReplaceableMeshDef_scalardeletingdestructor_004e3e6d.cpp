#pragma optimize("s", on)
extern void __fastcall CReplaceableMeshDef_Destruct(void* def);
void* __fastcall CReplaceableMeshDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CReplaceableMeshDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}