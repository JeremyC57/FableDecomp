// behaviour test for 0x009CA6C0 std::__copy over NGameText::CDataBankGroupEntry (model copy of the source)
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CIVCountedPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                  // +0x04 (PDB: ulong IVRefCount)
    void Release() const { if (--IVRefCount == 0) Destroy(); }
};
namespace NGameText { class CGroupEntry; }
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                     // +0x00 (PDB: CIVCountedPointeeBase* Object)
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};
namespace NGameText {
class CDataBankGroupEntry {                            // 4 bytes (PDB: CIVCountedPointer<CGroupEntry> PGroup)
public:
    CIVCountedPointer<CGroupEntry> PGroup;             // +0x00; the implicit operator= assigns it
};
}

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
class CPointee : public CIVCountedPointeeBase {
public:
    int id;
    virtual void v00() { }
    virtual void Destroy() const { ++g_destroyed; }
};
void CIVCountedPointeeBase::v00() { }
void CIVCountedPointeeBase::Destroy() const { }
static void Set(NGameText::CDataBankGroupEntry& e, CPointee* p) { e.PGroup.Object = p; if (p) ++p->IVRefCount; }
static CPointee* Get(const NGameText::CDataBankGroupEntry& e) { return (CPointee*)e.PGroup.Object; }
int main()
{
    CPointee a, b, c, d;
    a.IVRefCount = 0; b.IVRefCount = 0; c.IVRefCount = 0; d.IVRefCount = 0;
    NGameText::CDataBankGroupEntry src[3]; NGameText::CDataBankGroupEntry dst[4];
    for (int i = 0; i < 3; ++i) src[i].PGroup.Object = 0;
    for (int i = 0; i < 4; ++i) dst[i].PGroup.Object = 0;
    Set(src[0], &a); Set(src[1], &b); Set(src[2], &a);
    Set(dst[0], &c); Set(dst[1], &b); Set(dst[3], &d);   // dst[2] empty, dst[1] already b (self-equal skip)
    NGameText::CDataBankGroupEntry* end = std::__copy((NGameText::CDataBankGroupEntry*)src, (NGameText::CDataBankGroupEntry*)src + 3, dst, std::random_access_iterator_tag(), (int*)0);
    int ok = 1;
    if (end != dst + 3) ok = 0;
    if (Get(dst[0]) != &a || Get(dst[1]) != &b || Get(dst[2]) != &a || Get(dst[3]) != &d) ok = 0;
    if (a.IVRefCount != 4 || b.IVRefCount != 2 || c.IVRefCount != 0 || d.IVRefCount != 1) ok = 0;   // c released to zero
    if (g_destroyed != 1) ok = 0;
    // empty range: nothing touched
    end = std::__copy((NGameText::CDataBankGroupEntry*)src, (NGameText::CDataBankGroupEntry*)src, dst, std::random_access_iterator_tag(), (int*)0);
    if (end != dst || a.IVRefCount != 4) ok = 0;
    if (!ok) { std::printf("009ca6c0_TEST FAIL\n"); return 1; }
    std::printf("009ca6c0_TEST PASS\n");
    return 0;
}