struct CModeJackOfBladesScream {
    void* vftable;
};

void __fastcall CModeJackOfBladesScream_dtor(CModeJackOfBladesScream* mode);
void operator delete(void*);

void* __fastcall CModeJackOfBladesScream_scalar_deleting_destructor(CModeJackOfBladesScream* mode, int edx_unused, unsigned int flags)
{
    CModeJackOfBladesScream_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}