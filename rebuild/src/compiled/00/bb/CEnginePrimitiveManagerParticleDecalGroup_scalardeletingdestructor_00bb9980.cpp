class CEnginePrimitiveManagerParticleDecalGroup { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerParticleDecalGroup_dtor(CEnginePrimitiveManagerParticleDecalGroup* group);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerParticleDecalGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleDecalGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleDecalGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}