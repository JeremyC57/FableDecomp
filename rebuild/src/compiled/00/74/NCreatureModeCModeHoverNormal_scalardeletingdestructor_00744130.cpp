#pragma optimize("t", on)
extern void __fastcall DestructMember_00739db0(void* member);
extern void __fastcall CModeHoverNormal_Destruct(void* mode);
void* __fastcall CModeHoverNormal_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739db0(static_cast<unsigned char*>(mode) + 64);
    CModeHoverNormal_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}