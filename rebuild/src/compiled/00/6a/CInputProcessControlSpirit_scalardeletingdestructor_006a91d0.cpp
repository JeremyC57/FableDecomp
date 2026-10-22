#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CInputProcessControlSpirit_Destruct(void* process);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CInputProcessControlSpirit_CompositeScalarDeletingDestructor(void* process, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(process) + 40);
    CInputProcessControlSpirit_Destruct(process);
    if (flags & 1)
        OperatorDelete(process);
    return process;
}