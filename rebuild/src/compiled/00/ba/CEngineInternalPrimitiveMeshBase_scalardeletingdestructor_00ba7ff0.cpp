class CEngineInternalPrimitiveMeshBase { public:
    void* vftable;

public:
    virtual ~CEngineInternalPrimitiveMeshBase();   // 0x00BEBC50
};

void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveMeshBase_scalar_deleting_destructor(CEngineInternalPrimitiveMeshBase* mesh, int edx_unused, unsigned int flags)
{
    ((CEngineInternalPrimitiveMeshBase*)(mesh))->CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase();
    if (flags & 1)
        operator delete(mesh);
    return mesh;
}