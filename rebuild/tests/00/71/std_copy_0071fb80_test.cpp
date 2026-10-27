// behaviour test for 0x0071FB80 std::__copy over CDefPointer<const CTattooDef> (model copy of the source)
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1 (+0x04)
    mutable long RefCount;                             // +0x04
    void Release() const { if (--RefCount == 0) Destroy(); }
};
class CTattooDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                           // +0x00 (PDB: CDefPointeeBase* Object)
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
static int g_destroyed = 0;
class CPointee : public CDefPointeeBase {
public:
    int id;
    virtual void v00() { }
    virtual void Destroy() const { ++g_destroyed; }
};
void CDefPointeeBase::v00() { }
void CDefPointeeBase::Destroy() const { }
static void Set(CDefPointer<const CTattooDef>& e, CPointee* p) { e.Object = p; if (p) ++p->RefCount; }
static CPointee* Get(const CDefPointer<const CTattooDef>& e) { return (CPointee*)e.Object; }
int main()
{
    CPointee a, b, c, d;
    a.RefCount = 0; b.RefCount = 0; c.RefCount = 0; d.RefCount = 0;
    CDefPointer<const CTattooDef> src[3]; CDefPointer<const CTattooDef> dst[4];
    for (int i = 0; i < 3; ++i) src[i].Object = 0;
    for (int i = 0; i < 4; ++i) dst[i].Object = 0;
    Set(src[0], &a); Set(src[1], &b); Set(src[2], &a);
    Set(dst[0], &c); Set(dst[1], &b); Set(dst[3], &d);   // dst[2] empty, dst[1] already b (self-equal skip)
    CDefPointer<const CTattooDef>* end = std::__copy((CDefPointer<const CTattooDef>*)src, (CDefPointer<const CTattooDef>*)src + 3, dst, std::random_access_iterator_tag(), (int*)0);
    int ok = 1;
    if (end != dst + 3) ok = 0;
    if (Get(dst[0]) != &a || Get(dst[1]) != &b || Get(dst[2]) != &a || Get(dst[3]) != &d) ok = 0;
    if (a.RefCount != 4 || b.RefCount != 2 || c.RefCount != 0 || d.RefCount != 1) ok = 0;   // c released to zero
    if (g_destroyed != 1) ok = 0;
    // empty range: nothing touched
    end = std::__copy((CDefPointer<const CTattooDef>*)src, (CDefPointer<const CTattooDef>*)src, dst, std::random_access_iterator_tag(), (int*)0);
    if (end != dst || a.RefCount != 4) ok = 0;
    if (!ok) { std::printf("0071fb80_TEST FAIL\n"); return 1; }
    std::printf("0071fb80_TEST PASS\n");
    return 0;
}