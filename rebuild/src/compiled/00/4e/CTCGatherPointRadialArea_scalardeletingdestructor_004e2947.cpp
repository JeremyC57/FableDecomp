#pragma optimize("s", on)
extern void __fastcall CTCGatherPointRadialArea_Destruct(void* area);
void* __fastcall CTCGatherPointRadialArea_ScalarDeletingDestructor(void* area, int, unsigned int flags)
{
    CTCGatherPointRadialArea_Destruct(area);
    if (flags & 1)
        operator delete(area);
    return area;
}