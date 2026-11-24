#include <cstdio>

class CEnginePrimitivePosition { public:
    char pad[0xc];
    int payload;
};
class CMatrix3x4;
class C3DVector;
class CRightHandedSet;

static const void* g_self;
static const void* g_rawPos;
static const void* g_payloadPtr;
static float g_field58;

class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position, const CRightHandedSet& orientation, float scale);
};
void __fastcall CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& position,
        const CRightHandedSet& orientation, float scale) {
    g_self = &matrix; g_rawPos = &position; g_payloadPtr = &orientation; g_field58 = scale;
}

class CEngineInternalPrimitiveStaticMeshLayout { public:
    char pad0[0x58];
    float field58;
    char pad1[0x70 - 0x58 - 4];
    char sub;
};

// NF335 (NF324 Q2): the row now defines the PDB member CEngineInternalPrimitiveStaticMesh::SetPosition
// (?SetPosition@CEngineInternalPrimitiveStaticMesh@@UAEXABVCEnginePrimitivePosition@@@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineInternalPrimitiveStaticMesh { public:
    virtual void SetPosition(const CEnginePrimitivePosition&);
};

int main() {
    CEngineInternalPrimitiveStaticMeshLayout obj;
    obj.field58 = 2.5f;
    CEnginePrimitivePosition pos;
    pos.payload = 0x99;

    ((CEngineInternalPrimitiveStaticMesh*)(&obj))->CEngineInternalPrimitiveStaticMesh::SetPosition(pos);

    bool ok = (g_self == (void*)&obj.sub)
           && (g_rawPos == &pos)
           && (g_payloadPtr == (const void*)&pos.payload)
           && (g_field58 == 2.5f);
    if (ok) printf("00b9cfa0_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}