// Behaviour test for 0x00A8F1B0 C3DPrimitive2::ConvertToVBFormatStaticBump(CTVertexNormTex1Bump*): the row's defines, includes and
// body verbatim over the shared headers + the hand4d overlay (C3DPRIMITIVE2_CONVERTSTATICBUMP, the two CTVertexNormTex1Bump headers),
// run on zeroed C3DPrimitive2 / CDeviceIndependent images with Data pointing at the mesh. Covers: the vertex count from
// Data->Vertices (11 vertices: the four-times unrolled loop plus a remainder of 3; and 0 vertices: nothing written), each 0x30-byte
// destination written at its own index (position, normal, U, V, the U tangent of the same index), the V-tangent sign from
// (normal ^ u_tangent) . v_tangent >= 0 (positive, negative, exactly zero -> +1; the V tangent of the same index, not the U one), and
// the bytes after the last vertex untouched. No doubles needed (all inline).
#include <stdio.h>
#include <string.h>
#define private public
#define protected public
// ---- row body ----
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

// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static C3DVector V3(float x, float y, float z) { C3DVector v; v.X = x; v.Y = y; v.Z = z; return v; }

int main()
{
    static double prim_store[(sizeof(C3DPrimitive2) + 7) / 8];
    static double mesh_store[(sizeof(N3DPrimitive::CDeviceIndependent) + 7) / 8];
    memset(prim_store, 0, sizeof prim_store); memset(mesh_store, 0, sizeof mesh_store);
    C3DPrimitive2& prim = *(C3DPrimitive2*)prim_store;
    N3DPrimitive::CDeviceIndependent& mesh = *(N3DPrimitive::CDeviceIndependent*)mesh_store;
    *(N3DPrimitive::CDeviceIndependent**)&prim.Data = &mesh;

    const int N = 11;
    for (int i = 0; i < N; ++i) {
        C3DVertex2 v;
        v.Position = V3(1.0f + i, 2.0f + i, 3.0f + i);
        v.Normal = V3(0.0f, 0.0f, 1.0f);           // normal along Z
        v.U = 0.5f * i; v.V = 0.25f * i;
        mesh.Vertices.push_back(v);
        mesh.UTangents.push_back(V3(1.0f, 0.0f, 0.125f * i));   // normal ^ u = (0*t - 1*0, 1*1 - 0*t, 0), t = i/8 = (0, 1, 0)
        // v_tangent Y decides the sign: i % 3 == 0 -> +, 1 -> -, 2 -> exactly 0 (>= 0 -> +1)
        float vy = (i % 3 == 0) ? 2.0f : ((i % 3 == 1) ? -2.0f : 0.0f);
        mesh.VTangents.push_back(V3(5.0f, vy, 7.0f));
    }
    // (the U tangents have Y = 0, so reading them as the V tangent would give +1 at i % 3 == 1)
    CTVertexNormTex1Bump out[N + 1];
    memset(out, 0xCD, sizeof out);
    prim.ConvertToVBFormatStaticBump(out);
    for (int i = 0; i < N; ++i) {
        const CTVertexNormTex1Bump& o = out[i];
        check(o.X == 1.0f + i && o.Y == 2.0f + i && o.Z == 3.0f + i, "position");
        check(o.NormalX == 0.0f && o.NormalY == 0.0f && o.NormalZ == 1.0f, "normal");
        check(o.U1 == 0.5f * i && o.V1 == 0.25f * i, "U / V");
        check(o.UTangent[0] == 1.0f && o.UTangent[1] == 0.0f && o.UTangent[2] == 0.125f * i, "U tangent of the same index");
        float want = (i % 3 == 1) ? -1.0f : 1.0f;
        if (o.VTangentSign != want) { printf("FAIL sign at %d: %f want %f\n", i, o.VTangentSign, want); g_fail = 1; }
    }
    unsigned char* tailp = (unsigned char*)&out[N];
    int untouched = 1; for (unsigned k = 0; k < sizeof(CTVertexNormTex1Bump); ++k) if (tailp[k] != 0xCD) untouched = 0;
    check(untouched, "nothing written past the last vertex");

    // Sign from the cross product, not the normal alone: normal along X, u along Y -> normal ^ u = (0, 0, 1); v along -Z -> -1.
    mesh.Vertices.clear(); mesh.UTangents.clear(); mesh.VTangents.clear();
    { C3DVertex2 v; v.Position = V3(0, 0, 0); v.Normal = V3(1, 0, 0); v.U = 0; v.V = 0; mesh.Vertices.push_back(v); }
    mesh.UTangents.push_back(V3(0, 1, 0)); mesh.VTangents.push_back(V3(1, 1, -1));
    { C3DVertex2 v; v.Position = V3(0, 0, 0); v.Normal = V3(1, 0, 0); v.U = 0; v.V = 0; mesh.Vertices.push_back(v); }
    mesh.UTangents.push_back(V3(0, 1, 0)); mesh.VTangents.push_back(V3(-1, -1, 1));
    prim.ConvertToVBFormatStaticBump(out);
    check(out[0].VTangentSign == -1.0f, "(n ^ u) . v < 0 -> -1 (only Z of the cross is non-zero)");
    check(out[1].VTangentSign == 1.0f, "(n ^ u) . v > 0 -> +1");

    // zero vertices: nothing written
    mesh.Vertices.clear();
    memset(out, 0xCD, sizeof out);
    prim.ConvertToVBFormatStaticBump(out);
    check(((unsigned char*)out)[0] == 0xCD && ((unsigned char*)out)[0x2F] == 0xCD, "zero vertices: nothing written");

    if (!g_fail) printf("00a8f1b0_TEST PASS\n");
    return g_fail;
}
