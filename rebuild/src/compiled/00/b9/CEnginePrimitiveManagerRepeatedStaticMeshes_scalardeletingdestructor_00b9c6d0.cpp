struct CEnginePrimitiveManagerRepeatedStaticMeshes {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerRepeatedStaticMeshes_dtor(CEnginePrimitiveManagerRepeatedStaticMeshes* meshes);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerRepeatedStaticMeshes_scalar_deleting_destructor(CEnginePrimitiveManagerRepeatedStaticMeshes* meshes, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerRepeatedStaticMeshes_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}