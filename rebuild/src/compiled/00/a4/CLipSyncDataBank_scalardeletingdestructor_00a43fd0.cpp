struct CLipSyncDataBank {
    void* vftable;
};

void __fastcall CLipSyncDataBank_dtor(CLipSyncDataBank* bank);
void operator delete(void*);

void* __fastcall CLipSyncDataBank_scalar_deleting_destructor(CLipSyncDataBank* bank, int edx_unused, unsigned int flags)
{
    CLipSyncDataBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}