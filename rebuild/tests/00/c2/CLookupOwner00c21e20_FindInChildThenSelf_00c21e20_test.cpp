#include <cstdio>
struct CObject { int id; };
static CObject g_a, g_b;
struct CLookupNode {
    long hit; virtual CObject* Find(const long& key) { return key == hit ? &g_a : 0; }
};
struct CLookupOwner {
    CLookupNode m_child; long selfHit; int selfCalls;
    virtual CObject* Find(const long& key) { ++selfCalls; return key == selfHit ? &g_b : 0; }
    CObject* FindInChildThenSelf(long key)
    {
        CObject* found = m_child.Find(key);
        if (found == 0)
            found = Find(key);
        return found;
    }
};
int main()
{
    CLookupOwner o; o.m_child.hit = 3; o.selfHit = 5; o.selfCalls = 0;
    if (o.FindInChildThenSelf(3) != &g_a || o.selfCalls != 0) { std::printf("00c21e20_TEST FAIL\n"); return 1; }
    if (o.FindInChildThenSelf(5) != &g_b || o.selfCalls != 1) { std::printf("00c21e20_TEST FAIL\n"); return 1; }
    if (o.FindInChildThenSelf(9) != 0 || o.selfCalls != 2) { std::printf("00c21e20_TEST FAIL\n"); return 1; }
    std::printf("00c21e20_TEST PASS\n");
    return 0;
}