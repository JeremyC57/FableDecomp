struct C3DAnimFileLoader {
    void* vftable;
};

void __fastcall C3DAnimFileLoader_dtor(C3DAnimFileLoader* loader);
void operator delete(void*);

void* __fastcall C3DAnimFileLoader_scalar_deleting_destructor(C3DAnimFileLoader* loader, int edx_unused, unsigned int flags)
{
    C3DAnimFileLoader_dtor(loader);
    if (flags & 1)
        operator delete(loader);
    return loader;
}