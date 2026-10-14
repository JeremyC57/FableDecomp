#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() { m_pItem = 0; std::strcat(g_seq, "b"); } void* GetPItem() const { return m_pItem; } void SetPItem(void* item) { m_pItem = item; std::strcat(g_seq, "s"); } };
struct CIntelligentPointerA : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointerA() { } CIntelligentPointerA(const CIntelligentPointerA& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
struct CIntelligentPointerB : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointerB() { } CIntelligentPointerB(const CIntelligentPointerB& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); } };
struct SIcicleRecord { CIntelligentPointerA m_pointer0; long m_8; CIntelligentPointerB m_pointerc; long m_14; SIcicleRecord() { } SIcicleRecord(const SIcicleRecord& o) : m_pointer0(o.m_pointer0), m_8(o.m_8), m_pointerc(o.m_pointerc), m_14(o.m_14) { } };

int main() {
    SIcicleRecord src; std::memset(&src, 0, sizeof src); int a, b; src.m_pointer0.m_pItem = &a; src.m_8 = 8; src.m_pointerc.m_pItem = &b; src.m_14 = 14; g_seq[0] = 0; SIcicleRecord dst(src); if (std::strcmp(g_seq, "bsbs") || dst.m_pointer0.m_pItem != &a || dst.m_8 != 8 || dst.m_pointerc.m_pItem != &b || dst.m_14 != 14) { std::printf("0078e4a0_TEST FAIL\n"); return 1; }
    std::printf("0078e4a0_TEST PASS\n"); return 0;
}