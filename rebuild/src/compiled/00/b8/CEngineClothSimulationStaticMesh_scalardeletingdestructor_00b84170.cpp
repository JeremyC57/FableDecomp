struct CEngineClothSimulationStaticMesh {
    void* vftable;
};

void __fastcall CEngineClothSimulationStaticMesh_dtor(CEngineClothSimulationStaticMesh* mesh);
void operator delete(void*);

void* __fastcall CEngineClothSimulationStaticMesh_scalar_deleting_destructor(CEngineClothSimulationStaticMesh* mesh, int edx_unused, unsigned int flags)
{
    CEngineClothSimulationStaticMesh_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}