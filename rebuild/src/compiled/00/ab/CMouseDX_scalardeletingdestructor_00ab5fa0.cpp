struct CMouseDX {
    void* vftable;
};

void __fastcall CMouseDX_dtor(CMouseDX* mouseDX);
void operator delete(void*);

void* __fastcall CMouseDX_scalar_deleting_destructor(CMouseDX* mouseDX, int edx_unused, unsigned int flags)
{
    CMouseDX_dtor(mouseDX);
    if (flags & 1)
        operator delete(mouseDX);
    return mouseDX;
}