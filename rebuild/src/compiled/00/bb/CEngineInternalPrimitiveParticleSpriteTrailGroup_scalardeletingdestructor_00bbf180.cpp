struct CEngineInternalPrimitiveParticleSpriteTrailGroup {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveParticleSpriteTrailGroup_dtor(CEngineInternalPrimitiveParticleSpriteTrailGroup* group);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveParticleSpriteTrailGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleSpriteTrailGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveParticleSpriteTrailGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}