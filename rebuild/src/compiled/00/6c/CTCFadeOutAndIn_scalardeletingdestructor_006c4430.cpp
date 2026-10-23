#pragma optimize("t", on)
extern void __fastcall DestructMember_004406d0(void* member);
extern void __fastcall CTCFadeOutAndIn_Destruct(void* andIn);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCFadeOutAndIn_CompositeScalarDeletingDestructor(void* andIn, int, unsigned int flags)
{
    DestructMember_004406d0(static_cast<unsigned char*>(andIn) + 24);
    CTCFadeOutAndIn_Destruct(andIn);
    if (flags & 1)
        OperatorDelete(andIn);
    return andIn;
}