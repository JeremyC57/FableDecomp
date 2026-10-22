// CEnginePrimitiveRenderer::AddStaticMesh(...) -- forward the eleven arguments to the static-meshes manager at +0x280 and return its by-value result (the hidden return pointer is passed on and returned)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32F20, length 78 bytes.
// Callees: CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh @ 0x00BA0FC0 (on the manager; the same twelve stack words). Shape: the hidden pointer (the first
// stack word) is kept in esi and returned; the arguments are re-pushed last-first in eax/edx pairs with the manager load scheduled in the middle; `ret 30h`.
struct SHandle_00b32f20 { long m_id; long m_tag; };   // the by-value result (returned through a hidden pointer)

struct CEnginePrimitiveManagerStaticMeshesBase {
    SHandle_00b32f20 AddStaticMesh(long a, long b, long matrix, long d, long e, long f, long g, long h, long i, long j, long k);   // 0x00BA0FC0
};

struct CEnginePrimitiveRenderer {
    unsigned char                          _pad[0x280];
    CEnginePrimitiveManagerStaticMeshesBase* m_pStaticMeshes;   // +0x280
    SHandle_00b32f20 AddStaticMesh(long a, long b, long matrix, long d, long e, long f, long g, long h, long i, long j, long k);
};

SHandle_00b32f20 CEnginePrimitiveRenderer::AddStaticMesh(long a, long b, long matrix, long d, long e, long f, long g, long h, long i, long j, long k)
{
    return m_pStaticMeshes->AddStaticMesh(a, b, matrix, d, e, f, g, h, i, j, k);
}