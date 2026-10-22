#pragma optimize("s", on)
extern void __fastcall CTCDCameraPoint_Destruct(void* point);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDCameraPoint_ScalarDeletingDestructor(void* point, int, unsigned int flags)
{
    CTCDCameraPoint_Destruct(point);
    if (flags & 1)
        OperatorDelete(point);
    return point;
}