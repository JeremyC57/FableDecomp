#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CModeForcePushed_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeForcePushed_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(mode) + 28);
    CModeForcePushed_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}