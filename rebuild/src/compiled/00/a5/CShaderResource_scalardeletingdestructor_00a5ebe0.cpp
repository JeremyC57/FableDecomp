struct CShaderResource {
    void* vftable;
};

void __fastcall CShaderResource_dtor(CShaderResource* resource);
void operator delete(void*);

void* __fastcall CShaderResource_scalar_deleting_destructor(CShaderResource* resource, int edx_unused, unsigned int flags)
{
    CShaderResource_dtor(resource);
    if (flags & 1)
        operator delete(resource);
    return resource;
}