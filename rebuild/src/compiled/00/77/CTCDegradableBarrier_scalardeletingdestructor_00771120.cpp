struct CTCDegradableBarrier {
    void* vftable;
};

void __fastcall CTCDegradableBarrier_dtor(CTCDegradableBarrier* barrier);
void operator delete(void*);

void* __fastcall CTCDegradableBarrier_scalar_deleting_destructor(CTCDegradableBarrier* barrier, int edx_unused, unsigned int flags)
{
    CTCDegradableBarrier_dtor(barrier);
    if (flags & 1)
        operator delete(barrier);
    return barrier;
}