struct C3DMeshFileXLoader {
    void* vftable;
};

void __fastcall C3DMeshFileXLoader_dtor(C3DMeshFileXLoader* loader);
void operator delete(void*);

void* __fastcall C3DMeshFileXLoader_scalar_deleting_destructor(C3DMeshFileXLoader* loader, int edx_unused, unsigned int flags)
{
    C3DMeshFileXLoader_dtor(loader);
    if (flags & 1)
        operator delete(loader);
    return loader;
}