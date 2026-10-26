class CShaderBankEntry { public:
    void* vftable;
};

void __fastcall CShaderBankEntry_dtor(CShaderBankEntry* entry);
void operator delete(void*);

void* __fastcall CShaderBankEntry_scalar_deleting_destructor(CShaderBankEntry* entry, int edx_unused, unsigned int flags)
{
    CShaderBankEntry_dtor(entry);
    if (flags & 1)
        operator delete(entry);
    return entry;
}