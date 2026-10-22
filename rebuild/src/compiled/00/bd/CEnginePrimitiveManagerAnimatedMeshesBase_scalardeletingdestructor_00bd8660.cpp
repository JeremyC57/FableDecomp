struct CEnginePrimitiveManagerAnimatedMeshesBase {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerAnimatedMeshesBase_dtor(CEnginePrimitiveManagerAnimatedMeshesBase* meshes);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerAnimatedMeshesBase_scalar_deleting_destructor(CEnginePrimitiveManagerAnimatedMeshesBase* meshes, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerAnimatedMeshesBase_dtor(meshes);
    if (flags & 1)
        operator delete(meshes);
    return meshes;
}