#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CTCTextureDecal_Destruct(void* decal);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCTextureDecal_CompositeScalarDeletingDestructor(void* decal, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(decal) + 16);
    DestructSecondMember(static_cast<unsigned char*>(decal) + 12);
    CTCTextureDecal_Destruct(decal);
    if (flags & 1)
        OperatorDelete(decal);
    return decal;
}