#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_005142e0(void* member);
extern void __fastcall DestructSecondMember_004b96f0(void* member);
extern void __fastcall CTCFollowed_Destruct(void* followed);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCFollowed_CompositeScalarDeletingDestructor(void* followed, int, unsigned int flags)
{
    DestructFirstMember_005142e0(static_cast<unsigned char*>(followed) + 16);
    DestructSecondMember_004b96f0(static_cast<unsigned char*>(followed) + 12);
    CTCFollowed_Destruct(followed);
    if (flags & 1)
        OperatorDelete(followed);
    return followed;
}