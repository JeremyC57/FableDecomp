#pragma optimize("s", on)
extern void __fastcall CReplaceableMeshDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CReplaceableMeshDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CReplaceableMeshDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}