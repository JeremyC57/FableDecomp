#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionForcePushBall_Destruct(void* ball);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCShotDefinitionForcePushBall_ScalarDeletingDestructor(void* ball, int, unsigned int flags)
{
    CTCShotDefinitionForcePushBall_Destruct(ball);
    if (flags & 1)
        OperatorDelete(ball);
    return ball;
}