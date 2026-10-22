struct CShaderDirectionalLight {
    void* vftable;
};

void __fastcall CShaderDirectionalLight_dtor(CShaderDirectionalLight* light);
void operator delete(void*);

void* __fastcall CShaderDirectionalLight_scalar_deleting_destructor(CShaderDirectionalLight* light, int edx_unused, unsigned int flags)
{
    CShaderDirectionalLight_dtor(light);
    if (flags & 1)
        operator delete(light);
    return light;
}