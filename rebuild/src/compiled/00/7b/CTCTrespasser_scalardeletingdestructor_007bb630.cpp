#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CTCTrespasser_Destruct(void* trespasser);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTrespasser_CompositeScalarDeletingDestructor(void* trespasser, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(trespasser) + 56);
    DestructSecondMember(static_cast<unsigned char*>(trespasser) + 45);
    CTCTrespasser_Destruct(trespasser);
    if (flags & 1)
        OperatorDelete(trespasser);
    return trespasser;
}