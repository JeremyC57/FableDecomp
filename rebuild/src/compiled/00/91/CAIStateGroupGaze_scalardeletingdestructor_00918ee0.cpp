#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember_0099eae0(void* member);
extern void __fastcall CAIStateGroup_Gaze_Destruct(void* stateGroup);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAIStateGroup_Gaze_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(stateGroup) + 20);
    DestructSecondMember_0099eae0(static_cast<unsigned char*>(stateGroup) + 12);
    CAIStateGroup_Gaze_Destruct(stateGroup);
    if (flags & 1)
        OperatorDelete(stateGroup);
    return stateGroup;
}