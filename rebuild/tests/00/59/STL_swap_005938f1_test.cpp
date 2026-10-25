// Behaviour test for 0x005938F1 std::swap<pair<long,CDefPointer<const CContextSensitiveItemDef>>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("005938f1_TEST FAIL\n"); return 1; } } while (0)

static int g_deletes = 0;
class CDefPointeeBase {                                 // test model: intrusive count + Delete()
public:
    unsigned long IVRefCount;
    long          Priority;
    CDefPointeeBase() : IVRefCount(0), Priority(0) { }
    virtual ~CDefPointeeBase() { }
    virtual void Delete() const { ++g_deletes; }
};
template <class T>
class CDefPointer {
public:
    T* Object;
    CDefPointer(T* o) : Object(o) { if (Object) ++((CDefPointeeBase*)Object)->IVRefCount; }
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++((CDefPointeeBase*)Object)->IVRefCount; }
    ~CDefPointer() { if (Object) { if (--((CDefPointeeBase*)Object)->IVRefCount == 0) ((CDefPointeeBase*)Object)->Delete(); Object = 0; } }
    CDefPointer& operator=(const CDefPointer& p)
    {
        if (Object != p.Object) {
            if (Object) { if (--((CDefPointeeBase*)Object)->IVRefCount == 0) ((CDefPointeeBase*)Object)->Delete(); Object = 0; }
            Object = p.Object;
            if (Object) ++((CDefPointeeBase*)Object)->IVRefCount;
        }
        return *this;
    }
};
class CContextSensitiveItemDef : public CDefPointeeBase { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline void swap_model(_Tp& __a, _Tp& __b)    // STLport swap
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

int main()
{
    CContextSensitiveItemDef d1, d2;
    {
        pair<long, CDefPointer<CContextSensitiveItemDef> > a(3, CDefPointer<CContextSensitiveItemDef>(&d1));
        pair<long, CDefPointer<CContextSensitiveItemDef> > b(8, CDefPointer<CContextSensitiveItemDef>(&d2));
        CHECK(d1.IVRefCount == 1 && d2.IVRefCount == 1);
        swap_model(a, b);
        CHECK(a.first == 8 && a.second.Object == &d2 && b.first == 3 && b.second.Object == &d1);
        CHECK(d1.IVRefCount == 1 && d2.IVRefCount == 1 && g_deletes == 0);
    }
    CHECK(d1.IVRefCount == 0 && d2.IVRefCount == 0 && g_deletes == 2);
    std::printf("005938f1_TEST PASS\n");
    return 0;
}