#pragma optimize("s", on)
extern void __fastcall CTCSingleUserActionPoints_Destruct(void* points);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSingleUserActionPoints_ScalarDeletingDestructor(void* points, int, unsigned int flags)
{
    CTCSingleUserActionPoints_Destruct(points);
    if (flags & 1)
        OperatorDelete(points);
    return points;
}