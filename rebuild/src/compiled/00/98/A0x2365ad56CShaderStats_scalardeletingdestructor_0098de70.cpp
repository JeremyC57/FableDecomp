struct CShaderStats {
    void* vftable;
};

void __fastcall CShaderStats_dtor(CShaderStats* stats);
void operator delete(void*);

void* __fastcall CShaderStats_scalar_deleting_destructor(CShaderStats* stats, int edx_unused, unsigned int flags)
{
    CShaderStats_dtor(stats);
    if (flags & 1)
        operator delete(stats);
    return stats;
}