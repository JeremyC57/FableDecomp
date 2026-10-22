struct CEngineDynamic3DResource {
    void* vftable;
};

void __fastcall CEngineDynamic3DResource_dtor(CEngineDynamic3DResource* resource);
void operator delete(void*);

void* __fastcall CEngineDynamic3DResource_scalar_deleting_destructor(CEngineDynamic3DResource* resource, int edx_unused, unsigned int flags)
{
    CEngineDynamic3DResource_dtor(resource);
    if (flags & 1)
        operator delete(resource);
    return resource;
}