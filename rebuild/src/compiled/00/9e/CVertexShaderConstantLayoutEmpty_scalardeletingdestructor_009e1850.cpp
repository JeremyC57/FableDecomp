struct CVertexShaderConstantLayoutEmpty {
    void* vftable;
};

void __fastcall CVertexShaderConstantLayoutEmpty_dtor(CVertexShaderConstantLayoutEmpty* empty);
void operator delete(void*);

void* __fastcall CVertexShaderConstantLayoutEmpty_scalar_deleting_destructor(CVertexShaderConstantLayoutEmpty* empty, int edx_unused, unsigned int flags)
{
    CVertexShaderConstantLayoutEmpty_dtor(empty);
    if (flags & 1)
        operator delete(empty);
    return empty;
}