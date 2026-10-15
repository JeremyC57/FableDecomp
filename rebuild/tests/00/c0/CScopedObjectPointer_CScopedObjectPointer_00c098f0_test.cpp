#include <cstdio>
static int g_deleted = 0;
struct CDeletableObject {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { }
    virtual ~CDeletableObject() { ++g_deleted; }
};
struct CScopedObjectPointer {
    CDeletableObject* m_pObject;
    ~CScopedObjectPointer() { if (m_pObject) { delete m_pObject; m_pObject = 0; } }
};
int main()
{
    { CScopedObjectPointer p; p.m_pObject = new CDeletableObject; }
    { CScopedObjectPointer q; q.m_pObject = 0; }
    if (g_deleted != 1) { std::printf("00c098f0_TEST FAIL\n"); return 1; }
    std::printf("00c098f0_TEST PASS\n");
    return 0;
}