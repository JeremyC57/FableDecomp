#pragma optimize("s", on)
extern void __fastcall CTCDPhysicalObstruction_Destruct(void* obstruction);
void* __fastcall CTCDPhysicalObstruction_ScalarDeletingDestructor(void* obstruction, int, unsigned int flags)
{
    CTCDPhysicalObstruction_Destruct(obstruction);
    if (flags & 1)
        operator delete(obstruction);
    return obstruction;
}