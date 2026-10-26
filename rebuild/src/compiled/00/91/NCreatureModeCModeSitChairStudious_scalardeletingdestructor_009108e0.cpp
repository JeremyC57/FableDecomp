#pragma optimize("t", on)
extern void __fastcall DestructMember_00739df0(void* member);
extern void __fastcall CModeSitChairStudious_Destruct(void* mode);
void* __fastcall CModeSitChairStudious_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739df0(static_cast<unsigned char*>(mode) + 44);
    CModeSitChairStudious_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}