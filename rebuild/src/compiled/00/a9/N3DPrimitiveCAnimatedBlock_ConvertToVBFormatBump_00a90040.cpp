// N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump(CTVertexNormBlend4Tex1Bump* out, N3DPrimitive::CDeviceIndependent& mesh) -- fill the bump-mapped blended vertex buffer of one animated block: for every block vertex copy position / normal / uv from the mesh vertex it indexes, its bone indices (x3, palette slots) and weights zero-padded to four, the U tangent, and the handedness sign of the tangent frame
// Leaf: ?ConvertToVBFormatBump@CAnimatedBlock@N3DPrimitive@@AAEXPAVCTVertexNormBlend4Tex1Bump@@AAVCDeviceIndependent@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90040, length 403 bytes (+13 int3), __fastcall, `ret 8`. No callees (leaf: every
//   helper is expanded inline -- C3DVertex2::GetPosition / GetNormal / GetU / GetV, CGenericVertex::SetPosition / SetNormal / SetU1 /
//   SetV1, CArray::size / operator[], C3DVector::Cross / Dot). Float pool 0x0122DEDC = 0.0f.
// Name: Ego_r.pdb 0x00BD1EC0 and FableWin.pdb 0x017CE7D7 (private, non-const; the second argument is the MESH's
//   N3DPrimitive::CDeviceIndependent by reference, not the block's nested CDeviceIndependent). Only caller: 0x00A9130C in
//   C3DPrimitive2::ConvertToVBFormatAnimated (0x00A91180), the `flags & 2` branch.
// Layout: Ego_r.pdb CAnimatedBlock (BonesPerVertex +0x14, Data +0x2C -> VertexList +0x08, BoneWeights +0x14);
//   CTVertexNormBlend4Tex1BumpBase (56 bytes: X Y Z, BlendingIndices +0x0C, BlendingWeights +0x10, Normal +0x14, U1 V1 +0x20,
//   UTangent[3] +0x28, VTangentSign +0x34); the mesh's Vertices +0x04 and UTangents +0x38 (retail 12-byte CArrays).
// Shape: the landed same-TU sibling ConvertToVBFormatNonBump 0x00A8FF50 (one mesh-vertex reference per element -- read straight from Vertices: N3DPrimitive::CDeviceIndependent has no PeekVertex in
//   Ego_r.pdb / FableWin.pdb, only GetVertexCount / SetVertex / ResizeVertexList, the by-value `const C3DVector`
//   getters through the memberwise copy ctor with Z's dead frame store, the bone loop re-reading BonesPerVertex, the zero padding
//   from the same counter) plus the tangent tail of C3DPrimitive2::ConvertToVBFormatStaticBump 0x00A8F1B0. Retail quirk kept: the
//   handedness dot product is taken against the U tangent again (the mesh's VTangents at +0x44 is never read), so both tangent
//   references index UTangents. The vertex index is a named local: it is spilled across the bone loop and reused for the tangent.
// Header opt-ins: FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION selects the C3DVector::Cross body with all three components
//   parenthesised (same instructions as the default; the no-code PAREN tuples move VC7.1's scheduling-region cut, which is what
//   leaves the normal's Y copied with `fld st(1)` and popped after the Z component at 0x00A9017A..0x00A90182; the default body
//   consumes it with `fxch` and is 4 bytes shorter) and the Dot body X*oX+(Y*oY+Z*oZ) (FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
//   with that Cross matches as well). FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS / FABLE_RETAIL_CANIMATEDBLOCK_DATA: see the headers.
// No shared header exists for CTVertexNormBlend4Tex1BumpBase or the mesh's N3DPrimitive::CDeviceIndependent: modelled here from
//   Ego_r.pdb (the mesh as the landed constructor 0x00A98100 has it; the members this row does not touch are opaque bytes).
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CANIMATEDBLOCK_DATA
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
#include "engine/C3DVertex2.h"
#include "engine/C3DIndexedBoneWeight.h"
#include "engine/CArray.h"
#include "engine/CGenericVertex.h"
#include "engine/C3DPrimitive2.h"
class CTVertexNormBlend4Tex1BumpBase {                                   // Ego_r.pdb, 56 bytes
public:
    float X, Y, Z;                                                       // +0x00
    unsigned char BlendingIndices[4];                                    // +0x0C
    unsigned char BlendingWeights[4];                                    // +0x10
    float NormalX, NormalY, NormalZ;                                     // +0x14
    float U1, V1;                                                        // +0x20
    float UTangent[3];                                                   // +0x28
    float VTangentSign;                                                  // +0x34
};
class CTVertexNormBlend4Tex1Bump : public CGenericVertex<CTVertexNormBlend4Tex1BumpBase> { };
namespace N3DPrimitive {
class CDeviceIndependent : public CBaseClassNonCopyable {               // the mesh data (vtable 0x0129DFA8), retail 0x50 bytes
public:
    CArray<C3DVertex2> Vertices;                                         // +0x04
    unsigned char TriangleLists[12];                                     // +0x10 CArray (not used here)
    unsigned char Groups[12];                                            // +0x1C CArray (not used here)
    unsigned char BoundingSphere[16];                                    // +0x28 (not used here)
    CArray<C3DVector> UTangents;                                         // +0x38
    CArray<C3DVector> VTangents;                                         // +0x44
};
void CAnimatedBlock::ConvertToVBFormatBump(CTVertexNormBlend4Tex1Bump* out, N3DPrimitive::CDeviceIndependent& mesh)
{
    unsigned long count = Data->VertexList.size();
    for (unsigned long i = 0; i < count; ++i) {
        unsigned long index = Data->VertexList[i];
        const C3DVertex2& src = mesh.Vertices[index];
        CTVertexNormBlend4Tex1Bump& vertex = out[i];
        vertex.SetPosition(src.GetPosition());
        vertex.SetNormal(src.GetNormal());
        vertex.SetU1(src.GetU());
        vertex.SetV1(src.GetV());
        unsigned long j;
        for (j = 0; j < BonesPerVertex; ++j) {
            const C3DIndexedBoneWeight& weight = Data->BoneWeights[i * BonesPerVertex + j];
            vertex.BlendingIndices[j] = (unsigned char)(weight.BoneIndex * 3);
            vertex.BlendingWeights[j] = (unsigned char)weight.Weight;
        }
        for (; j < 4; ++j) {
            vertex.BlendingIndices[j] = 0;
            vertex.BlendingWeights[j] = 0;
        }
        const C3DVector& u_tangent = mesh.UTangents[index];
        const C3DVector& v_tangent = mesh.UTangents[index];              // retail: the U tangent again (VTangents is never read)
        vertex.UTangent[0] = u_tangent.X;
        vertex.UTangent[1] = u_tangent.Y;
        vertex.UTangent[2] = u_tangent.Z;
        C3DVector crossprod = src.GetNormal().Cross(u_tangent);
        if (crossprod.Dot(v_tangent) >= 0.0f) vertex.VTangentSign = 1.0f;
        else vertex.VTangentSign = -1.0f;
    }
}
}