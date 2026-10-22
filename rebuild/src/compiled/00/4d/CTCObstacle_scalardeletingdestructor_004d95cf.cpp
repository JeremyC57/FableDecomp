#pragma optimize("s", on)
extern void __fastcall CTCObstacle_Destruct(void* obstacle);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCObstacle_ScalarDeletingDestructor(void* obstacle, int, unsigned int flags)
{
    CTCObstacle_Destruct(obstacle);
    if (flags & 1)
        OperatorDelete(obstacle);
    return obstacle;
}