#pragma optimize("t", on)
extern void __fastcall DestructMember_00af7210(void* member);
extern void __fastcall CEngineInternalPrimitiveGroup_Destruct(void* group);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CEngineInternalPrimitiveGroup_CompositeScalarDeletingDestructor(void* group, int, unsigned int flags)
{
    DestructMember_00af7210(static_cast<unsigned char*>(group) + 80);
    CEngineInternalPrimitiveGroup_Destruct(group);
    if (flags & 1)
        OperatorDelete(group);
    return group;
}