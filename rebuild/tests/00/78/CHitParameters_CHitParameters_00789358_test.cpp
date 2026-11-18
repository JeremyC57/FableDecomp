#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CIVCountedPointeeBase { public: virtual void Destroy(int) { } virtual void Delete() { std::strcat(g_seq, "D"); } unsigned long IVRefCount; };
struct CIVCountedPointer { CIVCountedPointeeBase* Object; void Release() { CIVCountedPointeeBase* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; } } ~CIVCountedPointer() { Release(); } };
static void* g_freed = 0; static void FreeStub(void* p) { std::strcat(g_seq, "f"); g_freed = p; }
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; ~CBaseIntelligentPointer() { std::strcat(g_seq, "i"); } };
struct CMember12 { unsigned char _body[0xc]; ~CMember12() { std::strcat(g_seq, "M"); } };
struct CBuffer { void* m_pBlock; ~CBuffer() { if (m_pBlock) FreeStub(m_pBlock); } };
class CHitParameters { public: unsigned char _pad[0x30]; CMember12 m_member30; CBuffer m_buffer3c; unsigned char _pad40[0xc]; CIVCountedPointer m_counted4c; CBaseIntelligentPointer m_pointer50; CBaseIntelligentPointer m_pointer58; ~CHitParameters() { } };

int main() {
    CIVCountedPointeeBase a; a.IVRefCount = 1; { CHitParameters h; h.m_counted4c.Object = &a; h.m_buffer3c.m_pBlock = &a; } if (std::strcmp(g_seq, "iiDfM") || g_freed != &a) { std::printf("00789358_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; a.IVRefCount = 2; { CHitParameters h; h.m_counted4c.Object = &a; h.m_buffer3c.m_pBlock = 0; } if (std::strcmp(g_seq, "iiM") || a.IVRefCount != 1) { std::printf("00789358_TEST FAIL\n"); return 1; }
    std::printf("00789358_TEST PASS\n"); return 0;
}