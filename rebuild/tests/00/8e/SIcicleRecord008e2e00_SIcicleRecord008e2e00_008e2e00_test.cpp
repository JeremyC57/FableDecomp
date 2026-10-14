#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() { m_pItem = 0; std::strcat(g_seq, "b"); } void* GetPItem() const { return m_pItem; } void SetPItem(void* item) { m_pItem = item; std::strcat(g_seq, "s"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct SIcicleRecord { CIntelligentPointer m_pointer0, m_pointer8; long m_10, m_14; SIcicleRecord() : m_pointer0(), m_pointer8(), m_10(0), m_14(-1) { } };

int main() {
    SIcicleRecord r; if (std::strcmp(g_seq, "bb") || r.m_pointer0.m_pItem || r.m_pointer8.m_pItem || r.m_10 != 0 || r.m_14 != -1) { std::printf("008e2e00_TEST FAIL\n"); return 1; }
    std::printf("008e2e00_TEST PASS\n"); return 0;
}