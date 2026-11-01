#pragma optimize("t", on)
extern void __fastcall DestructFirstMember_00739dd0(void* member);
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
extern void __fastcall CModeCrouch_Destruct(void* mode);
void* __fastcall CModeCrouch_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructFirstMember_00739dd0(static_cast<unsigned char*>(mode) + 32);
    ((CCharString*)(static_cast<unsigned char*>(mode) + 28))->~CCharString();
    CModeCrouch_Destruct(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}