#pragma optimize("s", on)
extern void __fastcall CTCGatherPointBase_Destruct(void* point);
void* __fastcall CTCGatherPointBase_ScalarDeletingDestructor(void* point, int, unsigned int flags)
{
    CTCGatherPointBase_Destruct(point);
    if (flags & 1)
        operator delete(point);
    return point;
}