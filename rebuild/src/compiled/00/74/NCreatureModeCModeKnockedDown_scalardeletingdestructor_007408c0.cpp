#pragma optimize("t", on)
extern void __fastcall DestructMember_00739db0(void* member);
extern void __fastcall CModeKnockedDown_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeKnockedDown_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739db0(static_cast<unsigned char*>(mode) + 28);
    CModeKnockedDown_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}