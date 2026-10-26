#include <cstdio>
#include <cstring>
class CCharString { public: const char* p; CCharString(const char* s, long n) : p(s) { } CCharString(const CCharString& o) : p(o.p) { } };
struct CPSCRenderMesh { CCharString GetGUIName(); };
CCharString CPSCRenderMesh::GetGUIName() { return CCharString("Mesh Renderer", -1); }
int main() { CPSCRenderMesh o; bool ok = std::strcmp(o.GetGUIName().p, "Mesh Renderer") == 0; std::printf(ok ? "00b05a90_TEST PASS\n" : "00b05a90_TEST FAIL\n"); return ok ? 0 : 1; }
