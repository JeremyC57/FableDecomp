#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCHaste_Destruct(void* haste);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCHaste_CompositeScalarDeletingDestructor(void* haste, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(haste) + 24);
    CTCHaste_Destruct(haste);
    if (flags & 1)
        OperatorDelete(haste);
    return haste;
}