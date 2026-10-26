class CGraphicDataBank { public:
    void* vftable;
};

void __fastcall CGraphicDataBank_dtor(CGraphicDataBank* bank);
void operator delete(void*);

void* __fastcall CGraphicDataBank_scalar_deleting_destructor(CGraphicDataBank* bank, int edx_unused, unsigned int flags)
{
    CGraphicDataBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}