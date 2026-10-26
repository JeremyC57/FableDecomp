#pragma optimize("s", on)
extern void __fastcall CFireEffectCreationSet_Destruct(void* set);
void* __fastcall CFireEffectCreationSet_ScalarDeletingDestructor(void* set, int, unsigned int flags)
{
    CFireEffectCreationSet_Destruct(set);
    if (flags & 1)
        operator delete(set);
    return set;
}