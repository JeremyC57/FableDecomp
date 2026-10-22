#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CModeOutraged_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeOutraged_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(mode) + 104);
    CModeOutraged_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}