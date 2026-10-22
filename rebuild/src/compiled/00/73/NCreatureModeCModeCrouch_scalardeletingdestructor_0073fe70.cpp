#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CModeCrouch_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeCrouch_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(mode) + 32);
    DestructSecondMember(static_cast<unsigned char*>(mode) + 28);
    CModeCrouch_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}