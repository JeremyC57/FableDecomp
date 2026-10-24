// Behaviour test for 00592962: std::pair<long,CDefPointer<const CContextSensitiveItemDef> >::pair(const long&, const CDefPointer&).
#include <cstdio>

struct CDefPointeeBase {
    virtual ~CDefPointeeBase() { }
    mutable long m_refCount;
};
struct CContextSensitiveItemDef : CDefPointeeBase { };

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* p) : Object(p) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};

template <class T1, class T2> struct SPair {
    T1 first;
    T2 second;
    SPair(const T1& a, const T2& b) : first(a), second(b) { }
};

int main()
{
    CContextSensitiveItemDef def;
    def.m_refCount = 1;
    CDefPointer<const CContextSensitiveItemDef> p(&def);
    long key = 42;
    SPair<long, CDefPointer<const CContextSensitiveItemDef> > e(key, p);
    if (e.first != 42 || e.second.Object != &def || def.m_refCount != 2) { std::printf("00592962_TEST FAIL\n"); return 1; }
    CDefPointer<const CContextSensitiveItemDef> none(0);
    SPair<long, CDefPointer<const CContextSensitiveItemDef> > f(key, none);
    if (f.second.Object != 0 || def.m_refCount != 2) { std::printf("00592962_TEST FAIL\n"); return 1; }
    std::printf("00592962_TEST PASS\n");
    return 0;
}