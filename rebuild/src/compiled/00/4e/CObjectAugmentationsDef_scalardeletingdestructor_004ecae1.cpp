#pragma optimize("s", on)
extern void __fastcall CObjectAugmentationsDef_Destruct(void* def);
void* __fastcall CObjectAugmentationsDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CObjectAugmentationsDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}