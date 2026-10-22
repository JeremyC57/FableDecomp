#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionTearInGround_Destruct(void* ground);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionTearInGround_ScalarDeletingDestructor(void* ground, int, unsigned int flags)
{
    CTCShotDefinitionTearInGround_Destruct(ground);
    if (flags & 1)
        OperatorDelete(ground);
    return ground;
}