#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CZTargetNew_Destruct(void* targetNew);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CZTargetNew_CompositeScalarDeletingDestructor(void* targetNew, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(targetNew) + 164);
    CZTargetNew_Destruct(targetNew);
    if (flags & 1)
        OperatorDelete(targetNew);
    return targetNew;
}