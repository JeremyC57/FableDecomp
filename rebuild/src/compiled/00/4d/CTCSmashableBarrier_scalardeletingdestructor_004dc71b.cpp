#pragma optimize("s", on)
extern void __fastcall CTCSmashableBarrier_Destruct(void* barrier);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCSmashableBarrier_ScalarDeletingDestructor(void* barrier, int, unsigned int flags)
{
    CTCSmashableBarrier_Destruct(barrier);
    if (flags & 1)
        OperatorDelete(barrier);
    return barrier;
}