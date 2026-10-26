#pragma optimize("s", on)
extern void __fastcall CTCGatherPointBuilding_Destruct(void* building);
void* __fastcall CTCGatherPointBuilding_ScalarDeletingDestructor(void* building, int, unsigned int flags)
{
    CTCGatherPointBuilding_Destruct(building);
    if (flags & 1)
        operator delete(building);
    return building;
}