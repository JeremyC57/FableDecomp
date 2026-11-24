#include <stdio.h>

class CMatrix3x4 { public:
    float m[12];
};

struct CEngineInternalPrimitive2DStaticMeshLayout {
    char pad[0x70];
    CMatrix3x4 transform;
};

// NF335 (NF324 Q2): the row now defines the PDB member CEngineInternalPrimitive2DStaticMesh::GetObjectTransform
// (?GetObjectTransform@CEngineInternalPrimitive2DStaticMesh@@UBE_NAAVCMatrix3x4@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineInternalPrimitive2DStaticMesh { public:
    virtual bool GetObjectTransform(CMatrix3x4&) const;
};

int main() {
    CEngineInternalPrimitive2DStaticMeshLayout obj;
    for (int i = 0; i < 12; ++i) obj.transform.m[i] = (float)(i * 3 + 1);
    CMatrix3x4 dst;
    for (int i = 0; i < 12; ++i) dst.m[i] = -1.0f;
    bool r = ((CEngineInternalPrimitive2DStaticMesh*)(&obj))->CEngineInternalPrimitive2DStaticMesh::GetObjectTransform(dst);
    bool ok = r;
    for (int i = 0; i < 12; ++i) if (dst.m[i] != (float)(i * 3 + 1)) ok = false;
    if (ok) printf("00b9d070_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}