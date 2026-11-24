#include <cstdio>

class CMatrix3x4 { public: float m[12]; };

class CEngineInternalPrimitiveStaticMeshLayout { public:
    char pad[0x70];
    CMatrix3x4 transform;
};

// NF335 (NF324 Q2): the row now defines the PDB member CEngineInternalPrimitiveStaticMesh::GetObjectTransform
// (?GetObjectTransform@CEngineInternalPrimitiveStaticMesh@@UBE_NAAVCMatrix3x4@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineInternalPrimitiveStaticMesh { public:
    virtual bool GetObjectTransform(CMatrix3x4&) const;
};

int main() {
    CEngineInternalPrimitiveStaticMeshLayout obj;
    for (int i = 0; i < 12; ++i) obj.transform.m[i] = (float)(i + 1);
    CMatrix3x4 out;
    for (int i = 0; i < 12; ++i) out.m[i] = -1.0f;
    bool r = ((CEngineInternalPrimitiveStaticMesh*)(&obj))->CEngineInternalPrimitiveStaticMesh::GetObjectTransform(out);
    bool ok = r;
    for (int i = 0; i < 12; ++i) if (out.m[i] != (float)(i + 1)) ok = false;
    if (ok) printf("00b9cfc0_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}