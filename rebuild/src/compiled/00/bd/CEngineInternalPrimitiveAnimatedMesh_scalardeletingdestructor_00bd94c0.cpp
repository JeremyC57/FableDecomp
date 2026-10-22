struct CEngineInternalPrimitiveAnimatedMesh {
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveAnimatedMesh_dtor(CEngineInternalPrimitiveAnimatedMesh* mesh);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveAnimatedMesh_scalar_deleting_destructor(CEngineInternalPrimitiveAnimatedMesh* mesh, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveAnimatedMesh_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}