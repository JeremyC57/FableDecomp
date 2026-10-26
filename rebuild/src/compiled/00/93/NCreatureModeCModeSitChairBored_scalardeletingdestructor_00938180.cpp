#pragma optimize("t", on)
extern void __fastcall DestructMember_00739dd0(void* member);
extern void __fastcall CModeSitChairBored_Destruct(void* mode);
void* __fastcall CModeSitChairBored_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739dd0(static_cast<unsigned char*>(mode) + 44);
    CModeSitChairBored_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}