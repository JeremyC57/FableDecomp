#pragma optimize("t", on)
extern void __fastcall DestructMember_00a45d20(void* member);
extern void __fastcall C3DSkeletalMorphParameter_Destruct(void* parameter);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall C3DSkeletalMorphParameter_CompositeScalarDeletingDestructor(void* parameter, int, unsigned int flags)
{
    DestructMember_00a45d20(static_cast<unsigned char*>(parameter) + 12);
    C3DSkeletalMorphParameter_Destruct(parameter);
    if (flags & 1)
        OperatorDelete(parameter);
    return parameter;
}