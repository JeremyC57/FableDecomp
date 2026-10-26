#pragma optimize("s", on)
extern void __fastcall CTCSingleUserActionPoints_Destruct(void* points);
void* __fastcall CTCSingleUserActionPoints_ScalarDeletingDestructor(void* points, int, unsigned int flags)
{
    CTCSingleUserActionPoints_Destruct(points);
    if (flags & 1)
        operator delete(points);
    return points;
}