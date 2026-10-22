#pragma optimize("t", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CModeHandsBound_Destruct(void* mode);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CModeHandsBound_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(mode) + 104);
    CModeHandsBound_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}