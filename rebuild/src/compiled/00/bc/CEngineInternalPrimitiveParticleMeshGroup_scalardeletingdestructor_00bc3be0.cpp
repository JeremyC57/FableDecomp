struct CEngineInternalPrimitiveParticleMeshGroup {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveParticleMeshGroup_dtor(CEngineInternalPrimitiveParticleMeshGroup* group);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveParticleMeshGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleMeshGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveParticleMeshGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}