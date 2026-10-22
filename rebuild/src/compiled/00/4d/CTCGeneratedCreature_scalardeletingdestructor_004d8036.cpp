#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCGeneratedCreature_Destruct(void* creature);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCGeneratedCreature_CompositeScalarDeletingDestructor(void* creature, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(creature) + 12);
    CTCGeneratedCreature_Destruct(creature);
    if (flags & 1)
        OperatorDelete(creature);
    return creature;
}