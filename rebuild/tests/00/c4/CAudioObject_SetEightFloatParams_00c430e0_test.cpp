// Behaviour test for 0x00C430E0 CAudioObject set-or-clear record setter (flag 0x10)
#include <cstdio>
struct P { float a, b, c, d, e, f, g, h;
    bool operator==(const P& o) const { return a == o.a && b == o.b && c == o.c && a == o.a && e == o.e && f == o.f && g == o.g && h == o.h; } };
struct CAudioObject {
    unsigned int m_dirtyFlags; P* m_pRecord; P m_recordStorage;
    void Set(const P* value)
    {
        if (!value) { if (m_pRecord) { m_pRecord = 0; m_dirtyFlags |= 0x10; } return; }
        if (!m_pRecord) { m_pRecord = &m_recordStorage; m_recordStorage = *value; m_dirtyFlags |= 0x10; return; }
        if (!(*m_pRecord == *value)) { *m_pRecord = *value; m_dirtyFlags |= 0x10; }
    }
};
static int Fail() { std::printf("00c430e0_TEST FAIL\n"); return 1; }
int main()
{
    CAudioObject o; o.m_dirtyFlags = 0; o.m_pRecord = 0;
    o.Set(0);
    if (o.m_dirtyFlags != 0) return Fail();                          // clearing nothing marks nothing
    P v; v.a = 1; v.b = 2; v.c = 3; v.d = 4; v.e = 5; v.f = 6; v.g = 7; v.h = 8;
    o.Set(&v);
    if (o.m_pRecord != &o.m_recordStorage || o.m_dirtyFlags != 0x10 || !(o.m_recordStorage == v)) return Fail();
    o.m_dirtyFlags = 0; o.Set(&v);
    if (o.m_dirtyFlags != 0) return Fail();                          // same value: no change
    v.h = 9;
    o.Set(&v);
    if (o.m_dirtyFlags != 0x10 || !(o.m_recordStorage == v)) return Fail();
    o.m_dirtyFlags = 0; o.Set(0);
    if (o.m_pRecord != 0 || o.m_dirtyFlags != 0x10) return Fail();
    std::printf("00c430e0_TEST PASS\n");
    return 0;
}