struct CSoundBankLH {
    void* vftable;
};

void __fastcall CSoundBankLH_dtor(CSoundBankLH* bankLH);
void operator delete(void*);

void* __fastcall CSoundBankLH_scalar_deleting_destructor(CSoundBankLH* bankLH, int edx_unused, unsigned int flags)
{
    CSoundBankLH_dtor(bankLH);
    if (flags & 1)
        operator delete(bankLH);
    return bankLH;
}