#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCExclusiveUse_Destruct(void* use);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCExclusiveUse_CompositeScalarDeletingDestructor(void* use, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(use) + 12);
    CTCExclusiveUse_Destruct(use);
    if (flags & 1)
        OperatorDelete(use);
    return use;
}