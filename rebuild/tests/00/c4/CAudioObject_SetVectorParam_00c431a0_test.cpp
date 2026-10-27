// Behaviour test for 0x00C431A0 CAudioObject set-or-clear record setter (flag 0x20)
#include <cstdio>
struct P { float x, y, z;
    bool operator==(const P& o) const { return x == o.x && y == o.y && z == o.z; } };
struct CAudioObject {
    unsigned int m_dirtyFlags; P* m_pRecord; P m_recordStorage;
    void Set(const P* value)
    {
        if (!value) { if (m_pRecord) { m_pRecord = 0; m_dirtyFlags |= 0x20; } return; }
        if (!m_pRecord) { m_pRecord = &m_recordStorage; m_recordStorage = *value; m_dirtyFlags |= 0x20; return; }
        if (!(*m_pRecord == *value)) { *m_pRecord = *value; m_dirtyFlags |= 0x20; }
    }
};
static int Fail() { std::printf("00c431a0_TEST FAIL\n"); return 1; }
int main()
{
    CAudioObject o; o.m_dirtyFlags = 0; o.m_pRecord = 0;
    o.Set(0);
    if (o.m_dirtyFlags != 0) return Fail();                          // clearing nothing marks nothing
    P v; v.x = 1; v.y = 2; v.z = 3;
    o.Set(&v);
    if (o.m_pRecord != &o.m_recordStorage || o.m_dirtyFlags != 0x20 || !(o.m_recordStorage == v)) return Fail();
    o.m_dirtyFlags = 0; o.Set(&v);
    if (o.m_dirtyFlags != 0) return Fail();                          // same value: no change
    v.y = -2;
    o.Set(&v);
    if (o.m_dirtyFlags != 0x20 || !(o.m_recordStorage == v)) return Fail();
    o.m_dirtyFlags = 0; o.Set(0);
    if (o.m_pRecord != 0 || o.m_dirtyFlags != 0x20) return Fail();
    std::printf("00c431a0_TEST PASS\n");
    return 0;
}