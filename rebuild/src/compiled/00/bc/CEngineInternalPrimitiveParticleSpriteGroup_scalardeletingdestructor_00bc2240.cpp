struct CEngineInternalPrimitiveParticleSpriteGroup {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveParticleSpriteGroup_dtor(CEngineInternalPrimitiveParticleSpriteGroup* group);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveParticleSpriteGroup_scalar_deleting_destructor(CEngineInternalPrimitiveParticleSpriteGroup* group, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveParticleSpriteGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}