struct CTCDrainLife {
    void* vftable;
};

void __fastcall CTCDrainLife_dtor(CTCDrainLife* life);
void operator delete(void*);

void* __fastcall CTCDrainLife_scalar_deleting_destructor(CTCDrainLife* life, int edx_unused, unsigned int flags)
{
    CTCDrainLife_dtor(life);
    if (flags & 1)
        operator delete(life);
    return life;
}