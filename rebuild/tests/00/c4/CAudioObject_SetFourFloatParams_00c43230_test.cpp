// Behaviour test for 0x00C43230 CAudioObject set-or-clear record setter (flag 0x40)
#include <cstdio>
struct P { float a, b, c, d; int e;
    bool operator==(const P& o) const { return a == o.a && b == o.b && c == o.c && d == o.d && e == o.e; } };
struct CAudioObject {
    unsigned int m_dirtyFlags; P* m_pRecord; P m_recordStorage;
    void Set(const P* value)
    {
        if (!value) { if (m_pRecord) { m_pRecord = 0; m_dirtyFlags |= 0x40; } return; }
        if (!m_pRecord) { m_pRecord = &m_recordStorage; m_recordStorage = *value; m_dirtyFlags |= 0x40; return; }
        if (!(*m_pRecord == *value)) { *m_pRecord = *value; m_dirtyFlags |= 0x40; }
    }
};
static int Fail() { std::printf("00c43230_TEST FAIL\n"); return 1; }
int main()
{
    CAudioObject o; o.m_dirtyFlags = 0; o.m_pRecord = 0;
    o.Set(0);
    if (o.m_dirtyFlags != 0) return Fail();                          // clearing nothing marks nothing
    P v; v.a = 1; v.b = 2; v.c = 3; v.d = 4; v.e = 5;
    o.Set(&v);
    if (o.m_pRecord != &o.m_recordStorage || o.m_dirtyFlags != 0x40 || !(o.m_recordStorage == v)) return Fail();
    o.m_dirtyFlags = 0; o.Set(&v);
    if (o.m_dirtyFlags != 0) return Fail();                          // same value: no change
    v.e = 6;
    o.Set(&v);
    if (o.m_dirtyFlags != 0x40 || !(o.m_recordStorage == v)) return Fail();
    o.m_dirtyFlags = 0; o.Set(0);
    if (o.m_pRecord != 0 || o.m_dirtyFlags != 0x40) return Fail();
    std::printf("00c43230_TEST PASS\n");
    return 0;
}