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

class CEngineInternalPrimitiveStaticMesh { public:
    char pad0[0x58];
    float field58;
    char pad1[0x70 - 0x58 - 4];
    char sub;
};

void __fastcall CEngineInternalPrimitiveStaticMesh_SetPosition(
        CEngineInternalPrimitiveStaticMesh* self,
        int /*edx*/,
        const CEnginePrimitivePosition& pos);

int main() {
    CEngineInternalPrimitiveStaticMesh obj;
    obj.field58 = 2.5f;
    CEnginePrimitivePosition pos;
    pos.payload = 0x99;

    CEngineInternalPrimitiveStaticMesh_SetPosition(&obj, 0, pos);

    bool ok = (g_self == (void*)&obj.sub)
           && (g_rawPos == &pos)
           && (g_payloadPtr == (const void*)&pos.payload)
           && (g_field58 == 2.5f);
    if (ok) printf("00b9cfa0_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}