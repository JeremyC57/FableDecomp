class CEngineLayerRenderer { public:
    void* vftable;
};

void __fastcall CEngineLayerRenderer_dtor(CEngineLayerRenderer* renderer);
void operator delete(void*);

void* __fastcall CEngineLayerRenderer_scalar_deleting_destructor(CEngineLayerRenderer* renderer, int edx_unused, unsigned int flags)
{
    CEngineLayerRenderer_dtor(renderer);
    if (flags & 1)
        operator delete(renderer);
    return renderer;
}