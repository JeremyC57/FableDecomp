class CEngineScreenEffectDisplacementRenderer { public:
    void* vftable;
};

void __fastcall CEngineScreenEffectDisplacementRenderer_dtor(CEngineScreenEffectDisplacementRenderer* renderer);
void operator delete(void*);

void* __fastcall CEngineScreenEffectDisplacementRenderer_scalar_deleting_destructor(CEngineScreenEffectDisplacementRenderer* renderer, int edx_unused, unsigned int flags)
{
    CEngineScreenEffectDisplacementRenderer_dtor(renderer);
    if (flags & 1)
        operator delete(renderer);
    return renderer;
}