struct CEngineInternalPrimitiveParticleLineGroup {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveParticleLineGroup_dtor(CEngineInternalPrimitiveParticleLineGroup* group);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveParticleLineGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleLineGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveParticleLineGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}