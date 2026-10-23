#pragma optimize("t", on)
extern void __fastcall DestructMember_00874cc0(void* member);
extern void __fastcall CPCMouseLook_Destruct(void* look);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CPCMouseLook_CompositeScalarDeletingDestructor(void* look, int, unsigned int flags)
{
    DestructMember_00874cc0(static_cast<unsigned char*>(look) + 164);
    CPCMouseLook_Destruct(look);
    if (flags & 1)
        OperatorDelete(look);
    return look;
}