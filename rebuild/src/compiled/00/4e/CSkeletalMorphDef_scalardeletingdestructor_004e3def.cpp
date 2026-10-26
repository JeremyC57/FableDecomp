#pragma optimize("s", on)
extern void __fastcall CSkeletalMorphDef_Destruct(void* def);
void* __fastcall CSkeletalMorphDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CSkeletalMorphDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}