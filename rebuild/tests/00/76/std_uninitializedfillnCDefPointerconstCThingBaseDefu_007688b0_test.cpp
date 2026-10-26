// Test for 0x007688B0 std::uninitialized_fill_n<CDefPointer<const CThingBaseDef>*, ...>: n copies, each adding a reference.
#include <stdio.h>
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CDefPointeeBase {                                     // counted def object: vptr, reference count at +4
public:
    virtual void v00() { }
    virtual void Destroy() { }                            // slot 1
    long m_refCount;                                        // +0x04
};

class CThingBaseDef;

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                                // +0x00 (Ego_r.pdb)
    CDefPointer() { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                      const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

}   // namespace std

class CThingBaseDef : public CDefPointeeBase { };
int main()
{
    CThingBaseDef def; def.m_refCount = 1;
    CDefPointer<const CThingBaseDef> x; x.Object = &def;
    CDefPointer<const CThingBaseDef> raw[4]; raw[0].Object = raw[1].Object = raw[2].Object = raw[3].Object = 0;
    CDefPointer<const CThingBaseDef>* end = std::uninitialized_fill_n(raw, 3u, x);
    bool ok = end == raw + 3 && raw[2].Object == &def && raw[3].Object == 0 && def.m_refCount == 4;
    CDefPointer<const CThingBaseDef> nul; nul.Object = 0;
    ok = ok && std::uninitialized_fill_n(raw, 2u, nul) == raw + 2 && raw[0].Object == 0 && def.m_refCount == 4;
    ok = ok && std::uninitialized_fill_n(raw, 0u, x) == raw && def.m_refCount == 4;
    if (!ok) { printf("007688b0_TEST FAIL\n"); return 1; }
    printf("007688b0_TEST PASS\n");
    return 0;
}
