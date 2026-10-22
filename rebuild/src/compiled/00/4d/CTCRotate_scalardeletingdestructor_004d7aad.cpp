#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCRotate_Destruct(void* rotate);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCRotate_CompositeScalarDeletingDestructor(void* rotate, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(rotate) + 44);
    CTCRotate_Destruct(rotate);
    if (flags & 1)
        OperatorDelete(rotate);
    return rotate;
}