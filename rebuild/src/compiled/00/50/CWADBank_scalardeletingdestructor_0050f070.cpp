class CWADBank { public:
    void* vftable;
};

void __fastcall CWADBank_dtor(CWADBank* bank);
void operator delete(void*);

void* __fastcall CWADBank_scalar_deleting_destructor(CWADBank* bank, int edx_unused, unsigned int flags)
{
    CWADBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}