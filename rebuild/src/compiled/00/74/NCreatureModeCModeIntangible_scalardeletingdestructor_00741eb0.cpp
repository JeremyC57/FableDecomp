#pragma optimize("t", on)
extern void __fastcall DestructMember_0048c5c0(void* member);
extern void __fastcall CModeIntangible_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeIntangible_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_0048c5c0(static_cast<unsigned char*>(mode) + 60);
    CModeIntangible_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}