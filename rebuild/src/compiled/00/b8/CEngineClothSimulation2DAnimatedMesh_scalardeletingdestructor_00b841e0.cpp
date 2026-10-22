struct CEngineClothSimulation2DAnimatedMesh {
    void* vftable;
};

void __fastcall CEngineClothSimulation2DAnimatedMesh_dtor(CEngineClothSimulation2DAnimatedMesh* mesh);
void operator delete(void*);

void* __fastcall CEngineClothSimulation2DAnimatedMesh_scalar_deleting_destructor(CEngineClothSimulation2DAnimatedMesh* mesh, int edx_unused, unsigned int flags)
{
    CEngineClothSimulation2DAnimatedMesh_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}