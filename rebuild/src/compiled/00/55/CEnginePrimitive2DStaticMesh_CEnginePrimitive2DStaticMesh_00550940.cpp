// CEnginePrimitive2DStaticMesh::CEnginePrimitive2DStaticMesh -- constructs mesh index, clip box, alpha, orientation, invalid bounding sphere and tint; retail00550940 192bytes. PDB wrapper layout0x6c confirmed at offsets0xc/0x10/0x20/0x24/0x58/0x68; no matching wrapper or composite headers exist. Orientation copy installs class vtable01239824; other members copied fieldwise.
// Leaf: ??0CEnginePrimitive2DStaticMesh@@QAE@_KKABVC2DBoxF@@ABVC3DOrientationInterpolator@@W4EEngineInterpolationSet@@EVCRGBColour@@@Z
// Port (round 4, NF40): the local models of CEnginePrimitive2DStaticMesh, CEnginePrimitive, C2DBoxF, C3DOrientationInterpolator,
//   C3DBoundingSphere and CRGBColour (pointer parameters, a long interpolation set) are replaced by engine/CEnginePrimitive2DStaticMesh.h,
//   whose declaration is the PDB constructor ??0CEnginePrimitive2DStaticMesh@@QAE@_KKABVC2DBoxF@@ABVC3DOrientationInterpolator@@
//   W4EEngineInterpolationSet@@EVCRGBColour@@@Z (Ego_r.pdb / FableWin.pdb): references and the EEngineInterpolationSet enum.
//   The orientation copy is the compiler-generated one (FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GENERATED_COPY_PASS23), the clip box
//   the authored member-wise copy (FABLE_RETAIL_C2DBOXF_COPY), the invalid sphere C3DBoundingSphere(C3DVector(0,0,0), -1).
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GENERATED_COPY_PASS23
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#include "engine/EEnginePrimitiveType.h"
#include "engine/CEnginePrimitive2DStaticMesh.h"

CEnginePrimitive2DStaticMesh::CEnginePrimitive2DStaticMesh(unsigned __int64, unsigned long mesh, const C2DBoxF& box,
    const C3DOrientationInterpolator& orientation, EEngineInterpolationSet interpolation, unsigned char alpha, CRGBColour tint)
    : CEnginePrimitive(ENGINE_PRIMITIVE_2D_STATIC_MESH, interpolation), MeshIndex(mesh), ClipBox(box), Alpha(alpha),
      Orientation(orientation), BoundingSphere(C3DVector(0.0f, 0.0f, 0.0f), -1.0f), TintColour(tint)
{
}
