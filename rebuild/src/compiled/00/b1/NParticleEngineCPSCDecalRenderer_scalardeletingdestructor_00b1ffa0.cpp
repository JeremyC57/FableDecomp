struct CPSCDecalRenderer {
    void* vftable;
};

void __fastcall CPSCDecalRenderer_dtor(CPSCDecalRenderer* renderer);
void operator delete(void*);

void* __fastcall CPSCDecalRenderer_scalar_deleting_destructor(CPSCDecalRenderer* renderer, int edx_unused, unsigned int flags)
{
    CPSCDecalRenderer_dtor(renderer);
    if (flags & 1)
        operator delete(renderer);
    return renderer;
}