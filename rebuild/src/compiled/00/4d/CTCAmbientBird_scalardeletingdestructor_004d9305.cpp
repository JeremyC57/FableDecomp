#pragma optimize("s", on)
extern void __fastcall CTCAmbientBird_Destruct(void* bird);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCAmbientBird_ScalarDeletingDestructor(void* bird, int, unsigned int flags)
{
    CTCAmbientBird_Destruct(bird);
    if (flags & 1)
        OperatorDelete(bird);
    return bird;
}