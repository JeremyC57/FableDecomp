// Behaviour test for 0x0059308A pair<long, CDefPointer<const CContextSensitiveItemDef> >::operator= (implicit).
#include <cstdio>
#include <cstring>

namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}
class CDefCounted {
public:
    mutable long RefCount;
    mutable bool Destroyed;
    CDefCounted() : RefCount(1), Destroyed(false) {}
    virtual ~CDefCounted() {}
    virtual void Destroy() const { Destroyed = true; }
    void Release() const { if (--RefCount == 0) Destroy(); }
};
class CContextSensitiveItemDef : public CDefCounted { };
template <class T> class CDefPointer {
public:
    T* PDef;
    explicit CDefPointer(T* p) : PDef(p) { if (PDef) ++PDef->RefCount; }
    CDefPointer(const CDefPointer& other) : PDef(other.PDef) { if (PDef) ++PDef->RefCount; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (PDef != other.PDef) {
            if (PDef) { PDef->Release(); PDef = 0; }
            PDef = other.PDef;
            if (PDef) ++PDef->RefCount;
        }
        return *this;
    }
};
typedef stlp::pair<long, CDefPointer<const CContextSensitiveItemDef> > TDefPair;

int main()
{
    CContextSensitiveItemDef oldDef, newDef;
    TDefPair a(10, CDefPointer<const CContextSensitiveItemDef>(&oldDef));
    oldDef.RefCount = 1;                          // only the pair holds the old def now
    TDefPair b(20, CDefPointer<const CContextSensitiveItemDef>(&newDef));
    newDef.RefCount = 2;                          // the pair plus one outside owner
    TDefPair* r = &(a = b);
    TDefPair* self = &(b = b);                    // self-assignment keeps the count
    if (!(r == &a && a.first == 20 && a.second.PDef == &newDef && oldDef.Destroyed && oldDef.RefCount == 0 && newDef.RefCount == 3 && !newDef.Destroyed && self == &b && b.second.PDef == &newDef && newDef.RefCount == 3)) {
        std::printf("0059308a_TEST FAIL\n");
        return 1;
    }
    std::printf("0059308a_TEST PASS\n");
    return 0;
}