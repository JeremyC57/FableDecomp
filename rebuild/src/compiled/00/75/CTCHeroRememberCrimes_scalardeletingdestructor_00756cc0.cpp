#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCHeroRememberCrimes_Destruct(void* crimes);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCHeroRememberCrimes_CompositeScalarDeletingDestructor(void* crimes, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(crimes) + 12);
    CTCHeroRememberCrimes_Destruct(crimes);
    if (flags & 1)
        OperatorDelete(crimes);
    return crimes;
}