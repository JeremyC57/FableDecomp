#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CModeUnderwater_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeUnderwater_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(mode) + 104);
    CModeUnderwater_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}