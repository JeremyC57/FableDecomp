class CTCClock { public:
    void* vftable;
};

void __fastcall CTCClock_dtor(CTCClock* clock);
void operator delete(void*);

void* __fastcall CTCClock_scalar_deleting_destructor(CTCClock* clock, int edx_unused, unsigned int flags)
{
    CTCClock_dtor(clock);
    if (flags & 1)
        operator delete(clock);
    return clock;
}