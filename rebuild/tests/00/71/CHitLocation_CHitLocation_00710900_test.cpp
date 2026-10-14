#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CIVCountedPointeeBase { virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } unsigned long IVRefCount; ~CIVCountedPointeeBase() { std::strcat(g_seq, "P"); } };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } ~CIVCountedPointer() { Release(); } };
struct CHitLocation : CIVCountedPointeeBase { CIVCountedPointer m_pointer8; CIVCountedPointer m_pointerc; ~CHitLocation() { } };

int main() {
    CIVCountedPointeeBase a, b; a.IVRefCount = 1; b.IVRefCount = 1; { CHitLocation h; h.m_pointer8.Object = &a; h.m_pointerc.Object = &b; } if (std::strcmp(g_seq, "DDP")) { std::printf("00710900_TEST FAIL\n"); return 1; }
    std::printf("00710900_TEST PASS\n"); return 0;
}