#pragma optimize("s", on)
extern void __fastcall DestructMember_004406d0(void* member);
extern void __fastcall CAIStateGroup_WhisperPotionCombat_Destruct(void* stateGroup);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAIStateGroup_WhisperPotionCombat_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    DestructMember_004406d0(static_cast<unsigned char*>(stateGroup) + 28);
    CAIStateGroup_WhisperPotionCombat_Destruct(stateGroup);
    if (flags & 1)
        OperatorDelete(stateGroup);
    return stateGroup;
}