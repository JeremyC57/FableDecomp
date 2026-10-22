#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCProjectileTrap_Destruct(void* trap);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCProjectileTrap_CompositeScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(trap) + 108);
    CTCProjectileTrap_Destruct(trap);
    if (flags & 1)
        OperatorDelete(trap);
    return trap;
}