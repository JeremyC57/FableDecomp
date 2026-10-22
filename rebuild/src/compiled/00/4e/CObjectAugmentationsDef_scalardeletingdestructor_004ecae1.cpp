#pragma optimize("s", on)
extern void __fastcall CObjectAugmentationsDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CObjectAugmentationsDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CObjectAugmentationsDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}