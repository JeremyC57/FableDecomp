struct CTCHealLife {
    void* vftable;
};

void __fastcall CTCHealLife_dtor(CTCHealLife* life);
void operator delete(void*);

void* __fastcall CTCHealLife_scalar_deleting_destructor(CTCHealLife* life, int edx_unused, unsigned int flags)
{
    CTCHealLife_dtor(life);
    if (flags & 1)
        operator delete(life);
    return life;
}