struct CEnginePrimitiveManagerStaticMeshesBase {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerStaticMeshesBase_dtor(CEnginePrimitiveManagerStaticMeshesBase* meshes);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerStaticMeshesBase_scalar_deleting_destructor(CEnginePrimitiveManagerStaticMeshesBase* meshes, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerStaticMeshesBase_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}