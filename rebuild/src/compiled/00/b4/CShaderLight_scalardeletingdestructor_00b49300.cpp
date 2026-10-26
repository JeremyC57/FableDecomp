class CShaderLight { public:
    void* vftable;
};

void __fastcall CShaderLight_dtor(CShaderLight* light);
void operator delete(void*);

void* __fastcall CShaderLight_scalar_deleting_destructor(CShaderLight* light, int edx_unused, unsigned int flags)
{
    CShaderLight_dtor(light);
    if (flags & 1)
        operator delete(light);
    return light;
}