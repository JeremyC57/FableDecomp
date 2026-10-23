#pragma optimize("t", on)
extern void __fastcall DestructMember_0048d520(void* member);
extern void __fastcall CInputProcessCombat_Destruct(void* process);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CInputProcessCombat_CompositeScalarDeletingDestructor(void* process, int, unsigned int flags)
{
    DestructMember_0048d520(static_cast<unsigned char*>(process) + 40);
    CInputProcessCombat_Destruct(process);
    if (flags & 1)
        OperatorDelete(process);
    return process;
}