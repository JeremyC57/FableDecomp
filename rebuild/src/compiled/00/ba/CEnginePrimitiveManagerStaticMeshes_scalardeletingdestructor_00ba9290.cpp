class CEnginePrimitiveManagerStaticMeshes { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerStaticMeshes_dtor(CEnginePrimitiveManagerStaticMeshes* meshes);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerStaticMeshes_scalar_deleting_destructor(CEnginePrimitiveManagerStaticMeshes* meshes, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerStaticMeshes_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}