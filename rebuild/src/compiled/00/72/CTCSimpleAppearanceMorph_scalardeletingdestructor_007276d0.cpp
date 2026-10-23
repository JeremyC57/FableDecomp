#pragma optimize("t", on)
extern void __fastcall DestructMember_0071ee70(void* member);
extern void __fastcall CTCSimpleAppearanceMorph_Destruct(void* morph);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSimpleAppearanceMorph_CompositeScalarDeletingDestructor(void* morph, int, unsigned int flags)
{
    DestructMember_0071ee70(static_cast<unsigned char*>(morph) + 12);
    CTCSimpleAppearanceMorph_Destruct(morph);
    if (flags & 1)
        OperatorDelete(morph);
    return morph;
}