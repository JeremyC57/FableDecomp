#pragma optimize("s", on)
extern void __fastcall CTCDPhysicalObstruction_Destruct(void* obstruction);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDPhysicalObstruction_ScalarDeletingDestructor(void* obstruction, int, unsigned int flags)
{
    CTCDPhysicalObstruction_Destruct(obstruction);
    if (flags & 1)
        OperatorDelete(obstruction);
    return obstruction;
}