#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// std::vector<std::pair<unsigned long, CAnimationEntry>>::operator=(const vector&) -- (catalog "CAnimationSet::AssignAnimations") Dinkumware vector assignment of 20-byte (key, CAnimationEntry) pairs: reallocate when the source is larger than the capacity, otherwise copy over the live elements and destroy or uninitialised-copy the remainder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471865, length 239 bytes, ret 4, returns `this` (ebp frame, /Os code; not LTCG).
// Callees: _Ucopy/allocate-copy @ 0x0047197A (catalog "AnimationVector_AllocCopy"), _Tidy @ 0x00471A4F (catalog
// "CActiveFile::OnReadFinished", propagated), copy @ 0x004719CE (catalog "VectorInsert_CopyAnimations"; range end in edx),
// _Destroy_range @ 0x00454404 (__fastcall, iterator tag by reference), CAnimationEntry::CAnimationEntry(const CAnimationEntry&) @ 0x00471954.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CAnimationEntry {
    long BankIndex; long AnimName; long GroupName; void* Components;
    CAnimationEntry(const CAnimationEntry& other);              // 0x00471954
    CAnimationEntry& operator=(const CAnimationEntry& other);
};
struct SPair {
    unsigned long first; CAnimationEntry second;
    __forceinline SPair(const SPair& other) : first(other.first), second(other.second) {}
};
struct STag { };
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) throw() { return p; }
#else
#include <new>
#endif
SPair* __fastcall CopyRange(SPair* first, SPair* last, SPair* dest, const STag& tag);   // 0x004719CE
struct SAlloc { SAlloc() {} };
void __fastcall DestroyRange(SPair* first, SPair* last, const SAlloc& alloc);        // 0x00454404
__forceinline void Construct(SPair* p, const SPair& value) { new ((void*)p) SPair(value); }
__forceinline SPair* UninitCopy(SPair* first, SPair* last, SPair* dest)
{
    SPair* ptr = dest;
    for (; first != last; ++first, ++ptr)
        Construct(ptr, *first);
    return ptr;
}
struct CAnimationVector {
    SPair* _Myfirst; SPair* _Mylast; SPair* _Myend;
    SPair* AllocCopy(unsigned long count, SPair* first, SPair* last);   // 0x0047197A
    void _Tidy();                                                // 0x00471A4F
    unsigned long size() const { return _Mylast - _Myfirst; }
    unsigned long capacity() const { return _Myend - _Myfirst; }
    CAnimationVector& operator=(const CAnimationVector& right);
};
CAnimationVector& CAnimationVector::operator=(const CAnimationVector& right)
{
    if (&right != this) {
        unsigned long count = right.size();
        if (count > capacity()) {
            SPair* p = AllocCopy(count, right._Myfirst, right._Mylast);
            _Tidy();
            _Myfirst = p;
            _Myend = p + count;
        } else {
            if (size() >= count) {
                SPair* p = CopyRange(right._Myfirst, right._Mylast, _Myfirst, STag());
                DestroyRange(p, _Mylast, SAlloc());
            } else {
                CopyRange(right._Myfirst, right._Myfirst + size(), _Myfirst, STag());
                UninitCopy(right._Myfirst + size(), right._Mylast, _Mylast);
            }
        }
        _Mylast = _Myfirst + count;
    }
    return *this;
}

static int g_copies, g_assigns, g_destroyed, g_tidies, g_allocs;
CAnimationEntry::CAnimationEntry(const CAnimationEntry& other)
    : BankIndex(other.BankIndex), AnimName(other.AnimName), GroupName(other.GroupName), Components(other.Components) { ++g_copies; }
CAnimationEntry& CAnimationEntry::operator=(const CAnimationEntry& other)
{
    BankIndex = other.BankIndex; AnimName = other.AnimName; GroupName = other.GroupName; Components = other.Components;
    ++g_assigns;
    return *this;
}
SPair* __fastcall CopyRange(SPair* first, SPair* last, SPair* dest, const STag&)
{
    for (; first != last; ++first, ++dest) { dest->first = first->first; dest->second = first->second; }
    return dest;
}
void __fastcall DestroyRange(SPair* first, SPair* last, const SAlloc&) { g_destroyed += (int)(last - first); }
SPair* CAnimationVector::AllocCopy(unsigned long count, SPair* first, SPair* last)
{
    ++g_allocs;
    SPair* p = (SPair*)malloc(count * sizeof(SPair));
    SPair* d = p;
    for (; first != last; ++first, ++d) new ((void*)d) SPair(*first);
    return p;
}
void CAnimationVector::_Tidy() { ++g_tidies; free(_Myfirst); _Myfirst = _Mylast = _Myend = 0; }

static SPair* MakeArray(int capacity, int n, int base)
{
    SPair* p = (SPair*)calloc(capacity, sizeof(SPair));
    for (int i = 0; i < n; ++i) {
        p[i].first = base + i; p[i].second.BankIndex = base * 10 + i; p[i].second.AnimName = i;
        p[i].second.GroupName = -i; p[i].second.Components = 0;
    }
    return p;
}
static void Set(CAnimationVector& v, SPair* p, int n, int cap) { v._Myfirst = p; v._Mylast = p + n; v._Myend = p + cap; }
static int Same(const CAnimationVector& a, const CAnimationVector& b)
{
    if (a._Mylast - a._Myfirst != b._Mylast - b._Myfirst) return 0;
    for (int i = 0; i < (int)(a._Mylast - a._Myfirst); ++i)
        if (a._Myfirst[i].first != b._Myfirst[i].first || a._Myfirst[i].second.BankIndex != b._Myfirst[i].second.BankIndex
            || a._Myfirst[i].second.GroupName != b._Myfirst[i].second.GroupName) return 0;
    return 1;
}
int main()
{
    int ok = 1;
    CAnimationVector src, dst;
    // 1: reallocate (capacity 1 < 3)
    Set(src, MakeArray(3, 3, 100), 3, 3); Set(dst, MakeArray(1, 1, 7), 1, 1);
    CAnimationVector* r = &(dst = src);
    ok &= r == &dst && g_allocs == 1 && g_tidies == 1 && g_copies == 3 && Same(dst, src) && dst._Myend - dst._Myfirst == 3;
    // 2: shrink (size 3 >= 2): copy-assign 2, destroy 1
    CAnimationVector big; Set(big, MakeArray(4, 3, 200), 3, 4);
    CAnimationVector two; Set(two, MakeArray(2, 2, 300), 2, 2);
    g_copies = g_assigns = g_destroyed = 0;
    big = two;
    ok &= g_assigns == 2 && g_destroyed == 1 && g_copies == 0 && Same(big, two) && big._Myend - big._Myfirst == 4;
    // 3: grow within capacity (size 1 < 3 <= 5): assign 1, copy-construct 2
    CAnimationVector small; Set(small, MakeArray(5, 1, 400), 1, 5);
    g_copies = g_assigns = g_destroyed = 0;
    small = src;
    ok &= g_assigns == 1 && g_copies == 2 && g_destroyed == 0 && Same(small, src) && small._Myend - small._Myfirst == 5;
    // 4: self-assignment is a no-op
    g_copies = g_assigns = g_destroyed = g_allocs = 0;
    SPair* before = small._Myfirst;
    small = small;
    ok &= g_copies == 0 && g_assigns == 0 && g_allocs == 0 && small._Myfirst == before && small._Mylast - small._Myfirst == 3;
    printf(ok ? "00471865_TEST PASS\n" : "00471865_TEST FAIL\n");
    return ok ? 0 : 1;
}