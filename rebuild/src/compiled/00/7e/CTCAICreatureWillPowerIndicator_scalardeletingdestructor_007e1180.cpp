#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCAICreatureWillPowerIndicator_Destruct(void* indicator);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCAICreatureWillPowerIndicator_CompositeScalarDeletingDestructor(void* indicator, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(indicator) + 25);
    CTCAICreatureWillPowerIndicator_Destruct(indicator);
    if (flags & 1)
        OperatorDelete(indicator);
    return indicator;
}