// behaviour test for 0x00543FA0 GetDataFromDef: cache the first record's value (0 when empty) and release the def temporary
#include <cstdio>
static int g_destroyed = 0;
struct SDefRecord { int pad[15]; int m_value; int pad2[15]; };
struct CUIDef {
    int m_count; SDefRecord* first; SDefRecord* last;
    void Destroy() { ++g_destroyed; }
    void ReleaseTemporary() { --m_count; if (m_count == 0) Destroy(); }
};
struct CDefPointer {
    CUIDef* m_p;
    CDefPointer(CUIDef* p) : m_p(p) { ++m_p->m_count; }
    ~CDefPointer() { if (m_p) m_p->ReleaseTemporary(); }
    CUIDef* operator->() const { return m_p; }
};
struct CComponent {
    CUIDef* def; int m_defValue;
    CDefPointer GetDef() { return CDefPointer(def); }
    void GetDataFromDef()
    {
        CDefPointer d = GetDef();
        if (d->last - d->first) m_defValue = d->first[0].m_value; else m_defValue = 0;
    }
};
int main()
{
    SDefRecord r[2]; r[0].m_value = 42;
    CUIDef def = { 0, r, r + 2 }, empty = { 1, r, r };
    CComponent c; c.def = &def; c.m_defValue = -1; c.GetDataFromDef();
    CComponent e; e.def = &empty; e.m_defValue = -1; e.GetDataFromDef();
    if (c.m_defValue != 42 || e.m_defValue != 0 || g_destroyed != 1 || empty.m_count != 1) { std::printf("00543fa0_TEST FAIL\n"); return 1; }
    std::printf("00543fa0_TEST PASS\n");
    return 0;
}