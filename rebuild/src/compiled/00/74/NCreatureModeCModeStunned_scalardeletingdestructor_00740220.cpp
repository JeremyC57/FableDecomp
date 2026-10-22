#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CModeStunned_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeStunned_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(mode) + 120);
    CModeStunned_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}