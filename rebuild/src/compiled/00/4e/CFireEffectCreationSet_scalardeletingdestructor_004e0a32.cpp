#pragma optimize("s", on)
extern void __fastcall CFireEffectCreationSet_Destruct(void* set);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CFireEffectCreationSet_ScalarDeletingDestructor(void* set, int, unsigned int flags)
{
    CFireEffectCreationSet_Destruct(set);
    if (flags & 1)
        OperatorDelete(set);
    return set;
}