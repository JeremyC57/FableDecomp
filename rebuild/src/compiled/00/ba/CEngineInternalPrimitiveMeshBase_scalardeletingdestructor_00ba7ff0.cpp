class CEngineInternalPrimitiveMeshBase { public:
    void* vftable;
};

void __fastcall CEngineInternalPrimitiveMeshBase_dtor(CEngineInternalPrimitiveMeshBase* mesh);
void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveMeshBase_scalar_deleting_destructor(CEngineInternalPrimitiveMeshBase* mesh, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveMeshBase_dtor(mesh);
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}