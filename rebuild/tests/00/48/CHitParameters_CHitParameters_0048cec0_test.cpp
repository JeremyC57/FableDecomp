#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CIVCountedPointeeBase { public: virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } ~CIVCountedPointer() { Release(); } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; ~CBaseIntelligentPointer() { std::strcat(g_seq, "i"); } };
struct CHitParametersBase { unsigned char _body[0x28]; ~CHitParametersBase() { std::strcat(g_seq, "B"); } };
class CHitParameters : public CHitParametersBase { public: CBaseIntelligentPointer m_pointer28; CIVCountedPointer m_counted30; unsigned char _pad34[4]; CBaseIntelligentPointer m_pointer38; ~CHitParameters() { } };

int main() {
    CIVCountedPointeeBase a; a.IVRefCount = 1; { CHitParameters h; h.m_counted30.Object = &a; } if (std::strcmp(g_seq, "iDiB")) { std::printf("0048cec0_TEST FAIL\n"); return 1; }
    std::printf("0048cec0_TEST PASS\n"); return 0;
}