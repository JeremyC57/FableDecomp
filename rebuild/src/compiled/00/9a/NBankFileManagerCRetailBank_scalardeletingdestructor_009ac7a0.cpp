struct CRetailBank {
    void* vftable;
};

void __fastcall CRetailBank_dtor(CRetailBank* bank);
void operator delete(void*);

void* __fastcall CRetailBank_scalar_deleting_destructor(CRetailBank* bank, int edx_unused, unsigned int flags)
{
    CRetailBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}