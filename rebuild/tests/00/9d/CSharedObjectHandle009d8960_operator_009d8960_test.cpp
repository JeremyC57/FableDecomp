#include <cstdio>
// Behaviour test for 0x009D8960: the old object is released (deleted at zero), the new one shared and add-ref'd.
static int g_deleted = 0;
struct CSharedObject {
    virtual void DeleteThis(unsigned int flags) { if (flags == 1) ++g_deleted; }
    long m_refCount;
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) DeleteThis(1); }
};
struct CSharedObjectHandle_009d8960 {
    virtual void v00() {}
    CSharedObject* m_pObject;
    CSharedObjectHandle_009d8960& operator=(const CSharedObjectHandle_009d8960& other);
};
CSharedObjectHandle_009d8960& CSharedObjectHandle_009d8960::operator=(const CSharedObjectHandle_009d8960& other)
{
    if (m_pObject)
        m_pObject->Release();
    m_pObject = other.m_pObject;
    m_pObject->AddRef();
    return *this;
}
int main()
{
    CSharedObject a; a.m_refCount = 1;
    CSharedObject b; b.m_refCount = 2;
    CSharedObjectHandle_009d8960 h, g; h.m_pObject = &a; g.m_pObject = &b;
    CSharedObjectHandle_009d8960* r = &(h = g);
    if (r != &h || h.m_pObject != &b || b.m_refCount != 3 || a.m_refCount != 0 || g_deleted != 1) { std::printf("009d8960_TEST FAIL\n"); return 1; }
    CSharedObjectHandle_009d8960 e; e.m_pObject = 0;
    e = g;
    if (e.m_pObject != &b || b.m_refCount != 4 || g_deleted != 1) { std::printf("009d8960_TEST FAIL\n"); return 1; }
    std::printf("009d8960_TEST PASS\n");
    return 0;
}