#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CInputProcessCreatureMovement_Destruct(void* process);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CInputProcessCreatureMovement_CompositeScalarDeletingDestructor(void* process, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(process) + 40);
    CInputProcessCreatureMovement_Destruct(process);
    if (flags & 1)
        OperatorDelete(process);
    return process;
}