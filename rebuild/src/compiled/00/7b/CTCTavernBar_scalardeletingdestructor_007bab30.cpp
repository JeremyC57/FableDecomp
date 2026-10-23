#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember_007b9830(void* member);
extern void __fastcall CTCTavernBar_Destruct(void* bar);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTavernBar_CompositeScalarDeletingDestructor(void* bar, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(bar) + 24);
    DestructSecondMember_007b9830(static_cast<unsigned char*>(bar) + 12);
    CTCTavernBar_Destruct(bar);
    if (flags & 1)
        OperatorDelete(bar);
    return bar;
}