struct CEngineClothRenderer {
    void* vftable;
};

void __fastcall CEngineClothRenderer_dtor(CEngineClothRenderer* renderer);
void operator delete(void*);

void* __fastcall CEngineClothRenderer_scalar_deleting_destructor(CEngineClothRenderer* renderer, int edx_unused, unsigned int flags)
{
    CEngineClothRenderer_dtor(renderer);
    if (flags & 1)
        operator delete(renderer);
    return renderer;
}