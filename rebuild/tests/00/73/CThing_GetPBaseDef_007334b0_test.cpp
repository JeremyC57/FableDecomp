// Behaviour test for CThing::GetPBaseDef @ 0x007334B0: the by-value def pointer copy adds a reference when non-null.
#include <cstdio>
struct CDefPointee { long m_refCount; };
template <class T> class CDefPointer { public:
    T* m_p;
    explicit CDefPointer(T* p) : m_p(p) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_refCount; }
};
class CThing { public:
    CDefPointer<CDefPointee> PBaseDef;
    explicit CThing(CDefPointee* p) : PBaseDef(p) { }
    CDefPointer<CDefPointee> GetPBaseDef() const { return PBaseDef; }
};
int main()
{
    CDefPointee def; def.m_refCount = 1;
    CThing t(&def);
    CDefPointer<CDefPointee> r = t.GetPBaseDef();
    if (r.m_p != &def || def.m_refCount < 2) { std::printf("007334b0_TEST FAIL\n"); return 1; }
    CThing n(0);
    CDefPointer<CDefPointee> z = n.GetPBaseDef();
    if (z.m_p != 0) { std::printf("007334b0_TEST FAIL\n"); return 1; }
    std::printf("007334b0_TEST PASS\n");
    return 0;
}