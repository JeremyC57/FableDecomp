class CEnginePrimitiveManagerParticleSpriteGroup { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerParticleSpriteGroup_dtor(CEnginePrimitiveManagerParticleSpriteGroup* group);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerParticleSpriteGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleSpriteGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleSpriteGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}