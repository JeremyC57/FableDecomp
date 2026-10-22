#pragma optimize("s", on)
extern void __fastcall CTCGatherPointBuilding_Destruct(void* building);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCGatherPointBuilding_ScalarDeletingDestructor(void* building, int, unsigned int flags)
{
    CTCGatherPointBuilding_Destruct(building);
    if (flags & 1)
        OperatorDelete(building);
    return building;
}