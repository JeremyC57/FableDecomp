#include <cstdio>
static int g_destroyed = 0;
struct CRefCounted { long m_count; };
struct CDefVirtual { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } };
struct CDefBase : CDefVirtual, CRefCounted { void Release() { if (--m_count == 0) Destroy(); } };
struct CDefPointer {
    CDefBase* m_p;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) { m_p->Release(); m_p = 0; }
            m_p = other.m_p;
            if (m_p) ++m_p->m_count;
        }
        return *this;
    }
};
class CTCClock { public:
    CDefPointer PDef; float LastTimeUpdated;
    void Copy(const CTCClock* other) { PDef = other->PDef; }
};
int main()
{
    CDefBase a; a.m_count = 1; CDefBase b; b.m_count = 2;
    CTCClock x; x.PDef.m_p = &a; CTCClock y; y.PDef.m_p = &b;
    x.Copy(&y);
    if (x.PDef.m_p != &b || a.m_count != 0 || g_destroyed != 1 || b.m_count != 3) { std::printf("007327b0_TEST FAIL\n"); return 1; }
    x.Copy(&y);                                        /* same def: nothing changes */
    if (b.m_count != 3 || g_destroyed != 1) { std::printf("007327b0_TEST FAIL\n"); return 1; }
    CTCClock z; z.PDef.m_p = 0; x.Copy(&z);
    if (x.PDef.m_p != 0 || b.m_count != 2 || g_destroyed != 1) { std::printf("007327b0_TEST FAIL\n"); return 1; }
    std::printf("007327b0_TEST PASS\n");
    return 0;
}