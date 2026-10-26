// Behaviour test for 005f8578: std::pair<long,CDefPointer<const CHeroAbilityDef> > copy constructor.
#include <cstdio>

class CDefPointeeBase { public:
    virtual ~CDefPointeeBase() { }
    mutable long m_refCount;
};
class CHeroAbilityDef : public CDefPointeeBase { public: };

template <class T> class CDefPointer { public:
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
    SPair(const SPair& other) : first(other.first), second(other.second) { }
};

typedef SPair<long, CDefPointer<const CHeroAbilityDef> > TEntry;

int main()
{
    CHeroAbilityDef def;
    def.m_refCount = 3;
    CDefPointer<const CHeroAbilityDef> p(&def);
    TEntry a(7, p);                         // add-ref -> 4
    TEntry b(a);                            // add-ref -> 5
    if (b.first != 7 || b.second.Object != &def || def.m_refCount != 5) { std::printf("005f8578_TEST FAIL\n"); return 1; }
    CDefPointer<const CHeroAbilityDef> none(0);
    TEntry c(-2, none);
    TEntry d(c);                            // null def: no add-ref, no crash
    if (d.first != -2 || d.second.Object != 0 || def.m_refCount != 5) { std::printf("005f8578_TEST FAIL\n"); return 1; }
    std::printf("005f8578_TEST PASS\n");
    return 0;
}