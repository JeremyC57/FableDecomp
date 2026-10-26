class CTCUnholyPower { public:
    void* vftable;
};

void __fastcall CTCUnholyPower_dtor(CTCUnholyPower* power);
void operator delete(void*);

void* __fastcall CTCUnholyPower_scalar_deleting_destructor(CTCUnholyPower* power, int edx_unused, unsigned int flags)
{
    CTCUnholyPower_dtor(power);
    if (flags & 1)
        operator delete(power);
    return power;
}