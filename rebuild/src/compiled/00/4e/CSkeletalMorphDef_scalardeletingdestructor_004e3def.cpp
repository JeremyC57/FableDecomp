#pragma optimize("s", on)
extern void __fastcall CSkeletalMorphDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CSkeletalMorphDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CSkeletalMorphDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}