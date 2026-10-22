struct CEngineInternalPrimitive2DParticleGroup {
    void* vftable;
};

void __fastcall CEngineInternalPrimitive2DParticleGroup_dtor(CEngineInternalPrimitive2DParticleGroup* group);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitive2DParticleGroup_scalar_deleting_destructor(CEngineInternalPrimitive2DParticleGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitive2DParticleGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}