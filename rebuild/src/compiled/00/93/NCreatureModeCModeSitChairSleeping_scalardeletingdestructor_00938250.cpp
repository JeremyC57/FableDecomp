#pragma optimize("t", on)
extern void __fastcall DestructMember_00739dd0(void* member);
extern void __fastcall CModeSitChairSleeping_Destruct(void* mode);
void* __fastcall CModeSitChairSleeping_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739dd0(static_cast<unsigned char*>(mode) + 44);
    CModeSitChairSleeping_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}