#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() { m_pItem = 0; std::strcat(g_seq, "b"); } void* GetPItem() const { return m_pItem; } void SetPItem(void* item) { m_pItem = item; std::strcat(g_seq, "s"); } };
struct CIntelligentPointerA : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointerA() { } CIntelligentPointerA(const CIntelligentPointerA& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
struct CIntelligentPointerB : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointerB() { } CIntelligentPointerB(const CIntelligentPointerB& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
struct SIcicleRecord { CIntelligentPointerA m_pointer0; long m_8, m_c, m_10, m_14, m_18; SIcicleRecord() { } SIcicleRecord(const SIcicleRecord& o) : m_pointer0(o.m_pointer0), m_8(o.m_8), m_c(o.m_c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18) { } };

int main() {
    SIcicleRecord src; std::memset(&src, 0, sizeof src); int a, b; src.m_pointer0.m_pItem = &a; src.m_8 = 8; src.m_c = 12; src.m_10 = 16; src.m_14 = 20; src.m_18 = 24; (void)b; g_seq[0] = 0; SIcicleRecord dst(src); if (std::strcmp(g_seq, "bs") || dst.m_pointer0.m_pItem != &a || dst.m_8 != 8 || dst.m_c != 12 || dst.m_10 != 16 || dst.m_14 != 20 || dst.m_18 != 24) { std::printf("00806ae0_TEST FAIL\n"); return 1; }
    std::printf("00806ae0_TEST PASS\n"); return 0;
}