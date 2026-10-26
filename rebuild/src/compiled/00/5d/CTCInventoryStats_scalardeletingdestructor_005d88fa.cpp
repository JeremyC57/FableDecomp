#pragma optimize("s", on)
extern void __fastcall CTCInventoryStats_Destruct(void* stats);
void* __fastcall CTCInventoryStats_ScalarDeletingDestructor(void* stats, int, unsigned int flags)
{
    CTCInventoryStats_Destruct(stats);
    if (flags & 1)
        operator delete(stats);
    return stats;
}