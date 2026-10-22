struct CEnginePrimitiveManagerParticleLineGroup {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerParticleLineGroup_dtor(CEnginePrimitiveManagerParticleLineGroup* group);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerParticleLineGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleLineGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleLineGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}