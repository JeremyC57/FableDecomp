struct CEngineVSConstantLayoutParticles {
    void* vftable;
};

void __fastcall CEngineVSConstantLayoutParticles_dtor(CEngineVSConstantLayoutParticles* particles);
void operator delete(void*);

void* __fastcall CEngineVSConstantLayoutParticles_scalar_deleting_destructor(CEngineVSConstantLayoutParticles* particles, int edx_unused, unsigned int flags)
{
    CEngineVSConstantLayoutParticles_dtor(particles);
    if (flags & 1)
        operator delete(particles);
    return particles;
}