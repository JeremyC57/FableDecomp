#pragma optimize("s", on)
extern void __fastcall CTCGatherPointBase_Destruct(void* point);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCGatherPointBase_ScalarDeletingDestructor(void* point, int, unsigned int flags)
{
    CTCGatherPointBase_Destruct(point);
    if (flags & 1)
        OperatorDelete(point);
    return point;
}