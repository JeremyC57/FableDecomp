class CEnginePrimitiveManagerAnimatedMeshes { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerAnimatedMeshes_dtor(CEnginePrimitiveManagerAnimatedMeshes* meshes);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerAnimatedMeshes_scalar_deleting_destructor(CEnginePrimitiveManagerAnimatedMeshes* meshes, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerAnimatedMeshes_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}