struct CModeJackOfBladesFloat {
    void* vftable;
};

void __fastcall CModeJackOfBladesFloat_dtor(CModeJackOfBladesFloat* mode);
void operator delete(void*);

void* __fastcall CModeJackOfBladesFloat_scalar_deleting_destructor(CModeJackOfBladesFloat* mode, int edx_unused, unsigned int flags)
{
    CModeJackOfBladesFloat_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}