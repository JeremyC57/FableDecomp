struct CEngineInternalPrimitive2DAnimatedMesh {
    void* vftable;
};

void __fastcall CEngineInternalPrimitive2DAnimatedMesh_dtor(CEngineInternalPrimitive2DAnimatedMesh* mesh);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitive2DAnimatedMesh_scalar_deleting_destructor(CEngineInternalPrimitive2DAnimatedMesh* mesh, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitive2DAnimatedMesh_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}