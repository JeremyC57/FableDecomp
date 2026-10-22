#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CModeExplosiveForcePushed_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeExplosiveForcePushed_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(mode) + 40);
    DestructSecondMember(static_cast<unsigned char*>(mode) + 28);
    CModeExplosiveForcePushed_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}