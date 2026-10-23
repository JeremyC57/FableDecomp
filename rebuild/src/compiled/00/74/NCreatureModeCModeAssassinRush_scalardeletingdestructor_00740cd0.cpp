#pragma optimize("t", on)
extern void __fastcall DestructMember_00739db0(void* member);
extern void __fastcall CModeAssassinRush_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeAssassinRush_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739db0(static_cast<unsigned char*>(mode) + 104);
    CModeAssassinRush_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}