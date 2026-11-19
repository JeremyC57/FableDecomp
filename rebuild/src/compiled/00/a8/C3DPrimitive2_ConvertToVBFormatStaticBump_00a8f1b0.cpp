// C3DPrimitive2::ConvertToVBFormatStaticBump(CTVertexNormTex1Bump* dest) -- copy every device-independent vertex (position, normal, U, V) into the static bump vertex-buffer format with its U tangent, and the sign of the V tangent against normal x U tangent (1 when not behind, else -1)
// Grade: RELOCATION_MATCH (hand4d 2026-11-19, no flags). The parked Codex draft (its own FABLE_RETAIL_PRIMITIVE_BUMP header context;
//   PeekVertex, Cross; one `fnstsw ax` / `fstp st(0)` swap in the third unrolled iteration) is replaced by FableWin.pdb's text
//   (0x032F9EF0) over the shared headers: Data->GetVertexCount(), the VC6 for-scope idiom (FableWin `xor eax,eax ; je`
//   0x032FA081), `const C3DVertex2& src_vertex = Data->Vertices[c_vert]` (CArray operator[]), the by-value u_tangent / v_tangent
//   copies, `src_vertex.GetNormal() ^ u_tangent` (??TC3DVector) and Dot. Locals: Ego_d.pdb (no_vertices, c_vert, src_vertex,
//   dest_vertex, u_tangent, v_tangent, crossprod).
// Inferred: FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22 (operator^ with each component parenthesised, the donor 0x01B40A20
//   formal `vector`) is a fitted tie-breaker. Its parenthesis tuples put the fstp st(0) that drops the dead normal component after
//   the third unrolled iteration's fnstsw (0x00A8F4DE / 0x00A8F4E0). The default text leaves that swap (1469v1469, 2 lines).
//   Equally matching: CROSS_XYZ_GROUP (same text, formal `other`), DOT_ORDERED_ZYX and DOT_ORIGINAL_SCOPE_PASS23. Together with
//   DOT_ORIGINAL_SCOPE the loop is no longer unrolled (331 bytes).
// Overlay: engine/C3DPrimitive2.h FABLE_RETAIL_C3DPRIMITIVE2_CONVERTSTATICBUMP (new: CDeviceIndependent::GetVertexCount and this
//   member's declaration); engine/CTVertexNormTex1BumpBase.h and engine/CTVertexNormTex1Bump.h (new headers, Ego_r.pdb layout).
// Leaf: ?ConvertToVBFormatStaticBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1Bump@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8F1B0, 1469 bytes, thiscall `ret 4`. Name: Ego_r.pdb / FableWin.pdb
//   ?ConvertToVBFormatStaticBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1Bump@@@Z; the landed C3DPrimitive2::ConvertToVBFormatStatic
//   0x00A92740 calls it at `flags & 2`, else the landed sibling ConvertToVBFormatStaticNonBump 0x00A8EFA0. The catalog label
//   _global::TransformVertexBufferWithNormals_LoopUnrolled and the landed caller's TransformVertexBufferWithNormals(void*) are descriptive (NAME_FIXES.tsv). VC7.1 unrolls the loop four times.
// Callees: none (all inline).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DPRIMITIVE2_CONVERTSTATICBUMP
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS
#include "engine/C3DPrimitive2.h"
#include "engine/C3DVertex2.h"
#include "engine/CTVertexNormTex1Bump.h"

void C3DPrimitive2::ConvertToVBFormatStaticBump(CTVertexNormTex1Bump* dest)
{
    unsigned long no_vertices = Data->GetVertexCount();
    if (false) {} else for (unsigned long c_vert = 0; c_vert < no_vertices; ++c_vert) {
        const C3DVertex2& src_vertex = Data->Vertices[c_vert];
        CTVertexNormTex1Bump& dest_vertex = dest[c_vert];
        dest_vertex.SetPosition(src_vertex.GetPosition());
        dest_vertex.SetNormal(src_vertex.GetNormal());
        dest_vertex.SetU1(src_vertex.GetU());
        dest_vertex.SetV1(src_vertex.GetV());
        C3DVector u_tangent = Data->UTangents[c_vert];
        C3DVector v_tangent = Data->VTangents[c_vert];
        dest_vertex.UTangent[0] = u_tangent.X;
        dest_vertex.UTangent[1] = u_tangent.Y;
        dest_vertex.UTangent[2] = u_tangent.Z;
        C3DVector crossprod = src_vertex.GetNormal() ^ u_tangent;
        if (crossprod.Dot(v_tangent) >= 0.0f)
            dest_vertex.VTangentSign = 1.0f;
        else
            dest_vertex.VTangentSign = -1.0f;
    }
}