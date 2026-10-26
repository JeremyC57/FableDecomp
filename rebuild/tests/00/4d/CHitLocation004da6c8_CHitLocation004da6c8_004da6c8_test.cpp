#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CIVCountedPointeeBase { public: virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } ~CIVCountedPointer() { Release(); } };
struct CHitLocation { long m_0; CIVCountedPointer m_pointer4; CIVCountedPointer m_pointer8; ~CHitLocation() { } };

int main() {
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 2; { CHitLocation h; h.m_0 = 0; h.m_pointer4.Object = &a; h.m_pointer8.Object = &b; } if (std::strcmp(g_seq, "D") || b.IVRefCount != 1) { std::printf("004da6c8_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; { CHitLocation h; h.m_pointer4.Object = 0; h.m_pointer8.Object = &b; } if (std::strcmp(g_seq, "D") || b.IVRefCount != 0) { std::printf("004da6c8_TEST FAIL\n"); return 1; }
    std::printf("004da6c8_TEST PASS\n"); return 0;
}