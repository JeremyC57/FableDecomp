// C3DPrimitive2::ConvertToVBFormatStaticBump(CTVertexNormTex1Bump* dest) -- copy every device-independent vertex (position, normal, U, V) into the static bump vertex-buffer format with its U tangent, and the sign of the V tangent against normal x U tangent (1 when not behind, else -1)
// Grade: (in progress)
// Leaf: ?ConvertToVBFormatStaticBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1Bump@@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DPRIMITIVE2_CONVERTSTATICBUMP
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
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
