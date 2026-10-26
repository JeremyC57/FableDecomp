// CEnginePrimitiveManagerRepeatedStaticMeshes::UpdateBoneMatrixForWindSheer(CEngineInternalPrimitiveRepeatedStaticMesh*, C4DVector*, unsigned char*, long) -- for each object, write the delayed wind-sheer offset (X, Y of the history slot named by the object's delay byte, scaled by the primitive's WindScaleFactor) into the Z/W of its bone-matrix row
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B990B0, length 288 bytes (`ret 10h`, `this` unused).
// Callees: none. Globals: the engine system @ 0x01436E48 holds the wind-sheer history (12-byte C3DVector records) at +0x31c.
// Layout: engine/CEngineInternalPrimitiveRepeatedStaticMesh.h (WindScaleFactor +0xa0); the manager is engine/
// CEnginePrimitiveManagerRepeatedStaticMeshes.h (the method is attached through a derived shell). The row array is
// C4DVector (16-byte stride, Z +8, W +0xc).
// Shape: a plain counted `for` loop; VC7.1 unrolls it by four (index in edx, row pointer in eax) and finishes the
// remainder in a rolled tail. `fld [wind] ; fmul [scale]` = `wind.X * scale` (the member operand is not hoisted).
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"

class C3DVector { public:
    float X, Y, Z;
};
class C4DVector { public:
    float X, Y, Z, W;
};

struct SEngineSystem_01436e48 {
    unsigned char _pad[0x31c];
    C3DVector     WindSheerHistory[1];             // +0x31c (indexed by the per-object wind delay byte)
};
extern SEngineSystem_01436e48* g_pEngineSystem_01436e48;   // 0x01436E48

struct CEnginePrimitiveManagerRepeatedStaticMeshes_Wind : CEnginePrimitiveManagerRepeatedStaticMeshes {
    void UpdateBoneMatrixForWindSheer(CEngineInternalPrimitiveRepeatedStaticMesh* primitive, C4DVector* matrices,
                                      unsigned char* delays, long count);
};

void CEnginePrimitiveManagerRepeatedStaticMeshes_Wind::UpdateBoneMatrixForWindSheer(
    CEngineInternalPrimitiveRepeatedStaticMesh* primitive, C4DVector* matrices, unsigned char* delays, long count)
{
    float u0 = 0.0f;
    C3DVector* history = g_pEngineSystem_01436e48->WindSheerHistory;
    for (long i = 0; i < count; ++i) {
        const C3DVector& wind = history[delays[i]];
        matrices[i].Z = wind.X * primitive->WindScaleFactor;
        matrices[i].W = wind.Y * primitive->WindScaleFactor;
    }
}