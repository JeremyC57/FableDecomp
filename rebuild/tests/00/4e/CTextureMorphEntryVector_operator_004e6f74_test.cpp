// Behaviour test for 004e6f74: src/004e6f74.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTextureMorphEntryVector::operator=(const CTextureMorphEntryVector&)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTextureMorphEntryVector::operator=(const CTextureMorphEntryVector&) -- STLport vector<CTextureMorphEntry> assignment: reallocate when the source outgrows the capacity, else copy over the live elements and copy-construct the tail; returns *this
// Leaf: ??4CTextureMorphEntryVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6F74, length 331 bytes (`ret 4`); size-optimised def TU (`push N ; pop reg` constants, ebp frame).
// Pragma: #pragma optimize("s", on)
// Callees: CTextureMorphEntryVector::_M_allocate_and_copy @ 0x004E70CF and _M_clear @ 0x004E70C1 (both out of line; manifest labels
//          "CActiveFile::operator_assign" / "CActiveFile::OnReadFinished" are propagated across these byte-twin bodies).
// Shape: the landed CEngineGraphicVector::operator= (0x004E5F70) model: STLport 4.x `vector::operator=` verbatim over the
//          12-byte retail vector; `__copy` is the counted random-access loop (whole-element copies), `__uninitialized_copy`
//          placement-copies the tail through the element copy constructor, hence
//          the null test on the destination. The element is 24 bytes (`push 18h ; pop esi`, `lea eax,[edi+edi*2] ; shl 3`): whole-element
//          assignments are `rep movsd` (6), and the placement copy goes through a member-wise copy constructor (the first word, then the
//          20-byte block as `rep movsd` (5)). That user copy constructor also gives retail's frame, spills and rotated tail loop.
#pragma optimize("s", on)
#include <new>

struct CTextureMorphData { unsigned char Bytes[0x14]; };
struct CTextureMorphEntry {                  // 24 bytes: a word then a 20-byte block (the placement copy writes them apart)
    long              Index;                    // +0x00
    CTextureMorphData Data;                     // +0x04 (descriptive: four words + the morph value)
    CTextureMorphEntry(const CTextureMorphEntry& other) : Index(other.Index), Data(other.Data) {}
};

struct random_access_iterator_tag {};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                 _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        new (&*__cur) CTextureMorphEntry(*__first);
    return __cur;
}

struct CTextureMorphEntryVector {                     // retail std::vector<CTextureMorphEntry> (12 bytes)
    typedef CTextureMorphEntry* pointer;
    typedef const CTextureMorphEntry* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x004E70CF
    void _M_clear();                                                                              // 0x004E70C1
    CTextureMorphEntryVector& operator=(const CTextureMorphEntryVector& __x);
};

CTextureMorphEntryVector& CTextureMorphEntryVector::operator=(const CTextureMorphEntryVector& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            _M_start = __tmp;
            _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen) {
            __copy((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, _M_start, random_access_iterator_tag(), (int*)0);
        }
        else {
            __copy((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), _M_start, random_access_iterator_tag(), (int*)0);
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, _M_finish);
        }
        _M_finish = _M_start + __xlen;
    }
    return *this;
}

// ---- stubs ----
static long g_allocRaw[16 * 6];
#define g_alloc ((CTextureMorphEntry*)g_allocRaw)
static int g_allocCalls = 0, g_clearCalls = 0;
static unsigned int g_allocN = 0;
static const CTextureMorphEntry* g_allocFirst = 0;
static const CTextureMorphEntry* g_allocLast = 0;
CTextureMorphEntryVector::pointer CTextureMorphEntryVector::_M_allocate_and_copy(unsigned int n, const_pointer first, const_pointer last)
{
    ++g_allocCalls; g_allocN = n; g_allocFirst = first; g_allocLast = last;
    memcpy(g_alloc, first, (last - first) * sizeof(CTextureMorphEntry));
    return g_alloc;
}
void CTextureMorphEntryVector::_M_clear() { ++g_clearCalls; }

static void Fill(CTextureMorphEntry* p, int n, int base)
{
    for (int i = 0; i < n; ++i) {
        p[i].Index = base + i;
        for (int k = 0; k < 0x14; ++k)
            p[i].Data.Bytes[k] = (unsigned char)(base + i * 7 + k);
    }
}
static bool Same(const CTextureMorphEntry& a, const CTextureMorphEntry& b) { return memcmp(&a, &b, sizeof(a)) == 0; }

int main()
{
    char buf[64];
    static long srcRaw[6 * 6], dstRaw[8 * 6];
    CTextureMorphEntry* srcItems = (CTextureMorphEntry*)srcRaw;
    CTextureMorphEntry* dstItems = (CTextureMorphEntry*)dstRaw;
    Fill(srcItems, 6, 100);
    CTextureMorphEntryVector x;
    x._M_start = srcItems; x._M_finish = srcItems + 6; x._M_end_of_storage = srcItems + 6;

    // self assignment: nothing changes
    CTextureMorphEntryVector* r = &(x = x);
    CHECK(r == &x && x._M_finish == srcItems + 6 && g_allocCalls == 0);

    // grows past capacity: reallocate
    CTextureMorphEntryVector v;
    Fill(dstItems, 8, 1);
    v._M_start = dstItems; v._M_finish = dstItems + 2; v._M_end_of_storage = dstItems + 3;
    r = &(v = x);
    CHECK(r == &v);
    CHECK(g_allocCalls == 1 && g_clearCalls == 1 && g_allocN == 6);
    CHECK(g_allocFirst == srcItems && g_allocLast == srcItems + 6);
    CHECK(v._M_start == g_alloc && v._M_finish == g_alloc + 6 && v._M_end_of_storage == g_alloc + 6);

    // shrinks: copy over the live elements
    CTextureMorphEntryVector w;
    Fill(dstItems, 8, 1);
    w._M_start = dstItems; w._M_finish = dstItems + 8; w._M_end_of_storage = dstItems + 8;
    CTextureMorphEntryVector y;
    y._M_start = srcItems; y._M_finish = srcItems + 3; y._M_end_of_storage = srcItems + 6;
    w = y;
    CHECK(w._M_start == dstItems && w._M_finish == dstItems + 3 && w._M_end_of_storage == dstItems + 8);
    for (int i = 0; i < 3; ++i) CHECK(Same(dstItems[i], srcItems[i]));
    CHECK(dstItems[3].Index == 4);
    CHECK(g_allocCalls == 1 && g_clearCalls == 1);

    // grows within capacity: copy size() elements, construct the rest
    Fill(dstItems, 8, 1);
    w._M_start = dstItems; w._M_finish = dstItems + 2; w._M_end_of_storage = dstItems + 8;
    w = x;
    CHECK(w._M_finish == dstItems + 6);
    for (int i = 0; i < 6; ++i) CHECK(Same(dstItems[i], srcItems[i]));
    CHECK(dstItems[6].Index == 7);
    CHECK(g_allocCalls == 1);

    _snprintf(buf, sizeof(buf), "004e6f74_TEST %s", fails ? "FAIL" : "PASS");
    printf("%s\n", buf);
    return fails;
}