class CEnginePrimitiveManagerParticleSpriteTrailGroup { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerParticleSpriteTrailGroup_dtor(CEnginePrimitiveManagerParticleSpriteTrailGroup* group);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerParticleSpriteTrailGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleSpriteTrailGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleSpriteTrailGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}