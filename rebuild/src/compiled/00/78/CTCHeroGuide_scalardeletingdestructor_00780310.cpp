#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCHeroGuide_Destruct(void* guide);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCHeroGuide_CompositeScalarDeletingDestructor(void* guide, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(guide) + 12);
    CTCHeroGuide_Destruct(guide);
    if (flags & 1)
        OperatorDelete(guide);
    return guide;
}