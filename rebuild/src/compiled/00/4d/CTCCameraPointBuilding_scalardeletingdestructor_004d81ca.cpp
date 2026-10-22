#pragma optimize("s", on)
extern void __fastcall CTCCameraPointBuilding_Destruct(void* building);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCameraPointBuilding_ScalarDeletingDestructor(void* building, int, unsigned int flags)
{
    CTCCameraPointBuilding_Destruct(building);
    if (flags & 1)
        OperatorDelete(building);
    return building;
}