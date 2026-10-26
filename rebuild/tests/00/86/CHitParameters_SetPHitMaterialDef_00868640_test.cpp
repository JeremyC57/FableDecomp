#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CIVCountedPointeeBase { public: virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } CIVCountedPointer& operator=(const CIVCountedPointer& o) { if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->IVRefCount; } return *this; } ~CIVCountedPointer() { Release(); } };
class CHitParameters { public: unsigned char _pad[0x4c]; CIVCountedPointer m_hitMaterialDef; void SetPHitMaterialDef(CIVCountedPointer def) { m_hitMaterialDef = def; } };

int main() {
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1; CHitParameters h; h.m_hitMaterialDef.Object = &a; CIVCountedPointer arg; arg.Object = &b; ++b.IVRefCount;
    h.SetPHitMaterialDef(arg); if (h.m_hitMaterialDef.Object != &b || std::strcmp(g_seq, "D") || b.IVRefCount != 2) { std::printf("00868640_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; arg.Object = &b; ++b.IVRefCount; h.SetPHitMaterialDef(arg); if (h.m_hitMaterialDef.Object != &b || g_seq[0] || b.IVRefCount != 2) { std::printf("00868640_TEST FAIL\n"); return 1; }
    arg.Object = 0; h.SetPHitMaterialDef(arg); if (h.m_hitMaterialDef.Object != 0 || b.IVRefCount != 1 || g_seq[0]) { std::printf("00868640_TEST FAIL\n"); return 1; }
    std::printf("00868640_TEST PASS\n"); return 0;
}