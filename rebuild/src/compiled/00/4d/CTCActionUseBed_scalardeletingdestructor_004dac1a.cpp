#pragma optimize("s", on)
extern void __fastcall CTCActionUseBed_Destruct(void* bed);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseBed_ScalarDeletingDestructor(void* bed, int, unsigned int flags)
{
    CTCActionUseBed_Destruct(bed);
    if (flags & 1)
        OperatorDelete(bed);
    return bed;
}