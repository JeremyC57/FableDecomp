struct CTextureRenderer {
    void* vftable;
};

void __fastcall CTextureRenderer_dtor(CTextureRenderer* renderer);
void operator delete(void*);

void* __fastcall CTextureRenderer_scalar_deleting_destructor(CTextureRenderer* renderer, int edx_unused, unsigned int flags)
{
    CTextureRenderer_dtor(renderer);
    if (flags & 1)
        operator delete(renderer);
    return renderer;
}