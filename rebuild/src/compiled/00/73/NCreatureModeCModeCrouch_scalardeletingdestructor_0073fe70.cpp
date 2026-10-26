#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_00739dd0(void* member);
extern void __fastcall DestructSecondMember_0099eae0(void* member);
extern void __fastcall CModeCrouch_Destruct(void* mode);
void* __fastcall CModeCrouch_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructFirstMember_00739dd0(static_cast<unsigned char*>(mode) + 32);
    DestructSecondMember_0099eae0(static_cast<unsigned char*>(mode) + 28);
    CModeCrouch_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}