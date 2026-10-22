#pragma optimize("t", on)
extern void __fastcall DestructFirstMember(void* member);
extern void __fastcall DestructSecondMember(void* member);
extern void __fastcall CModeDragonGetHit_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeDragonGetHit_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(mode) + 116);
    DestructSecondMember(static_cast<unsigned char*>(mode) + 104);
    CModeDragonGetHit_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}