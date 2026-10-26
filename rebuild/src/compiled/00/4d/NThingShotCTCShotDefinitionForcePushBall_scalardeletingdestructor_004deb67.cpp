#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionForcePushBall_Destruct(void* ball);
void* __fastcall CTCShotDefinitionForcePushBall_ScalarDeletingDestructor(void* ball, int, unsigned int flags)
{
    CTCShotDefinitionForcePushBall_Destruct(ball);
    if (flags & 1)
        operator delete(ball);
    return ball;
}