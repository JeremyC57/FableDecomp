#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionTearInGround_Destruct(void* ground);
void* __fastcall CTCShotDefinitionTearInGround_ScalarDeletingDestructor(void* ground, int, unsigned int flags)
{
    CTCShotDefinitionTearInGround_Destruct(ground);
    if (flags & 1)
        operator delete(ground);
    return ground;
}