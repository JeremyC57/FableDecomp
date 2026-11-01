// Behaviour test for CEnginePrimitiveManagerDecal::~CEnginePrimitiveManagerDecal() @ 0x00BBBBC0: the implicit destructor
// tears the decal buffers down last-member-first (landscape index buffer first, mesh vertex buffer last), then the base.
#include <cstdio>
#include <string>
namespace model {
std::string g_order;
struct CVertexBufferWin32 { const char* n; CVertexBufferWin32(const char* name) : n(name) {} void Uninitialise() { g_order += n; g_order += ';'; } ~CVertexBufferWin32() { Uninitialise(); } };
struct CIndexBuffer { const char* n; CIndexBuffer(const char* name) : n(name) {} void Uninitialise() { g_order += n; g_order += ';'; } ~CIndexBuffer() { Uninitialise(); } };
struct CEnginePrimitiveManager { void* SceneList; bool Enable; virtual ~CEnginePrimitiveManager() { g_order += "Manager;"; } };
struct CEnginePrimitiveManagerDecal : CEnginePrimitiveManager {
    CVertexBufferWin32 MeshDecalVertexBuffer; CIndexBuffer MeshDecalIndexBuffer; CVertexBufferWin32 LandscapeDecalVertexBuffer; CIndexBuffer LandscapeDecalIndexBuffer;
    CEnginePrimitiveManagerDecal() : MeshDecalVertexBuffer("MeshVB"), MeshDecalIndexBuffer("MeshIB"), LandscapeDecalVertexBuffer("LandVB"), LandscapeDecalIndexBuffer("LandIB") {}
};
}
int main()
{
    { model::CEnginePrimitiveManagerDecal decal; }
    bool ok = model::g_order == "LandIB;LandVB;MeshIB;MeshVB;Manager;";
    if (!ok) { std::printf("00bbbbc0_TEST FAIL %s\n", model::g_order.c_str()); return 1; }
    std::printf("00bbbbc0_TEST PASS\n");
    return 0;
}