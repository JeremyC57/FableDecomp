// C3DPrimitive2::ConvertToVBFormatStaticBump(CTVertexNormTex1Bump *) -- Copy vertices and tangent handedness.
// Leaf: ?ConvertToVBFormatStaticBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1Bump@@@Z
// Grade: DIFFER(1469v1469) -- adjacent 'fnstsw ax' / 'fstp st(0)' order swapped at +0x32E/+0x330 in the third unrolled iteration (scheduler tie); 11 shapes (loop test/count forms, post-increment, signed counter, setter order, handedness local, dot local, float* stores, tangent references, /Op) leave or worsen it. Graded in Codex's header context (FABLE_RETAIL_PRIMITIVE_BUMP per-feature macro = header debt)
#define FABLE_RETAIL_PRIMITIVE_BUMP
#define FABLE_RETAIL_CMOVABLERESOURCE
#include "engine/C3DPrimitive2.h"
void C3DPrimitive2::ConvertToVBFormatStaticBump(CTVertexNormTex1Bump* dest)
{
    unsigned long no_vertices=Data->GetVertexCount();
    for(unsigned long c_vert=0;c_vert<no_vertices;++c_vert) {
        const C3DVertex2& src_vertex=Data->PeekVertex(c_vert);
        CTVertexNormTex1Bump& dest_vertex=dest[c_vert];
        dest_vertex.SetPosition(src_vertex.GetPosition());
        dest_vertex.SetNormal(src_vertex.GetNormal());
        dest_vertex.SetU1(src_vertex.GetU());
        dest_vertex.SetV1(src_vertex.GetV());
        C3DVector u_tangent=Data->UTangents[c_vert];
        C3DVector v_tangent=Data->VTangents[c_vert];
        dest_vertex.UTangent[0]=u_tangent.X;
        dest_vertex.UTangent[1]=u_tangent.Y;
        dest_vertex.UTangent[2]=u_tangent.Z;
        C3DVector crossprod=src_vertex.GetNormal().Cross(u_tangent);
        if(crossprod.Dot(v_tangent)>=0.0f) dest_vertex.VTangentSign=1.0f;
        else dest_vertex.VTangentSign=-1.0f;
    }
}
