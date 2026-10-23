#pragma optimize("s", on)
extern void __fastcall DestructMember_004db68f(void* member);
extern void __fastcall CTCAnimatingObject_Destruct(void* animatingObject);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCAnimatingObject_CompositeScalarDeletingDestructor(void* animatingObject, int, unsigned int flags)
{
    DestructMember_004db68f(static_cast<unsigned char*>(animatingObject) + 20);
    CTCAnimatingObject_Destruct(animatingObject);
    if (flags & 1)
        OperatorDelete(animatingObject);
    return animatingObject;
}