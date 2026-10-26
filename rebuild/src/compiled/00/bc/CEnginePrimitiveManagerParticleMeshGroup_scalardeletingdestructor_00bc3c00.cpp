class CEnginePrimitiveManagerParticleMeshGroup { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerParticleMeshGroup_dtor(CEnginePrimitiveManagerParticleMeshGroup* group);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerParticleMeshGroup_scalar_deleting_destructor(CEnginePrimitiveManagerParticleMeshGroup* group, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerParticleMeshGroup_dtor(group);
    if (flags & 1)
        operator delete(group);
    return group;
}