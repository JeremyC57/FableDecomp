// Standalone behaviour test for CEngineInternalPrimitiveMeshBase::GetMorphInfo @ 0x00ba7fe0
#include <stdio.h>

class CEngineInternalPrimitiveMorphInfo;

class CEngineInternalPrimitiveMeshBaseLayout { public: int filler; };

// NF335 (NF324 Q2): the row now defines the PDB member CEngineInternalPrimitiveMeshBase::GetMorphInfo
// (?GetMorphInfo@CEngineInternalPrimitiveMeshBase@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineInternalPrimitiveMeshBase { public:
    virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();
};

int main(void)
{
    CEngineInternalPrimitiveMeshBaseLayout mesh;
    mesh.filler = 0x11223344;

    CEngineInternalPrimitiveMorphInfo * mi =
        ((CEngineInternalPrimitiveMeshBase*)(&mesh))->CEngineInternalPrimitiveMeshBase::GetMorphInfo();

    if (mi == 0)
        printf("CENGINEINTERNALPRIMITIVEMESHBASE_00ba7fe0_TEST PASS\n");
    else
        printf("FAIL: expected null morph info, got %p\n", (void*)mi);

    return 0;
}
