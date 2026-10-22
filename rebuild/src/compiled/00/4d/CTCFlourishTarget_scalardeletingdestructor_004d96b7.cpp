#pragma optimize("s", on)
extern void __fastcall DestructMember(void* member);
extern void __fastcall CTCFlourishTarget_Destruct(void* target);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCFlourishTarget_CompositeScalarDeletingDestructor(void* target, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(target) + 12);
    CTCFlourishTarget_Destruct(target);
    if (flags & 1)
        OperatorDelete(target);
    return target;
}