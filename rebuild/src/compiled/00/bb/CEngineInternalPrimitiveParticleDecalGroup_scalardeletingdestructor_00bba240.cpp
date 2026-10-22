struct CEngineInternalPrimitiveParticleDecalGroup {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveParticleDecalGroup_dtor(CEngineInternalPrimitiveParticleDecalGroup* group);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveParticleDecalGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleDecalGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveParticleDecalGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}