#include <cstdio>
#include <cstring>

class CMatrix3x4 { public:
    float m[12];
};

class CEngineInternalPrimitiveStaticMeshLayout { public:
    char pad[0x70];
    CMatrix3x4 frame;
};

// NF335 (NF324 Q2): the row now defines the PDB member CEngineInternalPrimitiveStaticMesh::GetTransformFrameStart
// (?GetTransformFrameStart@CEngineInternalPrimitiveStaticMesh@@UBE_NAAVCMatrix3x4@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineInternalPrimitiveStaticMesh { public:
    virtual bool GetTransformFrameStart(CMatrix3x4&) const;
};

int main() {
    CEngineInternalPrimitiveStaticMeshLayout obj;
    memset(&obj, 0, sizeof(obj));
    for (int i = 0; i < 12; ++i) obj.frame.m[i] = (float)(i + 1) * 1.5f;

    CMatrix3x4 out;
    memset(&out, 0, sizeof(out));
    bool r = ((CEngineInternalPrimitiveStaticMesh*)(&obj))->CEngineInternalPrimitiveStaticMesh::GetTransformFrameStart(out);

    bool ok = r;
    for (int i = 0; i < 12; ++i)
        if (out.m[i] != (float)(i + 1) * 1.5f) ok = false;

    if (ok) printf("00b9d000_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}