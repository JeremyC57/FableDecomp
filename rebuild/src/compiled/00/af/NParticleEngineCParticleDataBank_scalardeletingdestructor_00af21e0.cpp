struct CParticleDataBank {
    void* vftable;
};

void __fastcall CParticleDataBank_dtor(CParticleDataBank* bank);
void operator delete(void*);

void* __fastcall CParticleDataBank_scalar_deleting_destructor(CParticleDataBank* bank, int edx_unused, unsigned int flags)
{
    CParticleDataBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}