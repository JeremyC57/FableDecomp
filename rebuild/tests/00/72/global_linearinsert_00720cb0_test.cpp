// Behaviour test for 00720cb0: src/00720cb0.cpp copied verbatim; the insertion-sort step puts `value` first (shifting
// [first, last) up one slot) when its key sorts before the first key, else sinks it from `last` with the unguarded insert.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// _global::__linear_insert(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord value, SKeyLess comp) -- SGI insertion-sort step: if `value` sorts before the first element, shift [first, last) up one slot and put it first, otherwise sink it from `last` with __unguarded_linear_insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00720CB0, length 121 bytes (`ret 1Ch`: the 24-byte value and the comparator on
// the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's "CDateAndTime::CDateAndTime" is propagated over byte-identical skeletons and wrong.
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, Distance*) @ 0x0071EDD0 (manifest
// "_Move_backward<pair<unsigned __int64, CPersistInfo>*>" is propagated), __unguarded_linear_insert(last, value, comp)
// @ 0x0071F5E0 (`ret 1Ch`, 24-byte records keyed by their first long), the record's 16-byte tail copy constructor
// @ 0x005BA48A (manifest "CCompositeBlendDesc::CCompositeBlendDesc" is propagated across byte-identical bodies).
// Element: 24 bytes {long key; long value; a 16-byte tail with a user copy constructor}; the comparator is an empty
// functor comparing the signed keys (`cmp ebx,[esi] ; jge`).
// Shape: the copy_backward front end is inlined: its empty iterator tag is a const-ref temporary homed in the dead
// comparator slot (`lea eax,[esp+2Ch] ; push eax`) with a null Distance*; the record's implicit assignment copies the six
// words from the argument slots; on the other branch the by-value record is rebuilt in the argument area (key and value
// words, then the tail's out-of-line copy constructor) after the comparator is pushed.
// The tail's copy constructor gets a same-TU `auto_inline(off)` stand-in body that writes only *this. VC7.1 then knows
//   value.first survives it and reads the key once into ebx, as retail (an extern declaration re-reads the argument slot).
struct SRecordTail {                              // 16 bytes, user copy constructor (out of line)
    long          m_00;
    long          m_04;
    unsigned char m_bytes[8];
    SRecordTail(const SRecordTail& other);        // 0x005BA48A
};

#pragma auto_inline(off)
SRecordTail::SRecordTail(const SRecordTail& other)   // 0x005BA48A (same-TU stand-in: writes only *this)
    : m_00(other.m_00), m_04(other.m_04)
{
    for (int i = 0; i < 8; ++i)
        m_bytes[i] = other.m_bytes[i];
}
#pragma auto_inline(on)
struct SRecordInfo {                              // 20 bytes
    long        value;                            // +0x00
    SRecordTail tail;                             // +0x04
};

struct SKeyedRecord {                             // STLport std::pair<long, SRecordInfo>, 24 bytes
    long        first;                            // +0x00 key
    SRecordInfo second;                           // +0x04
    SKeyedRecord(const SKeyedRecord& o) : first(o.first), second(o.second) { }   // STLport's explicit pair copy constructor
};

struct SKeyLess {                                 // CKeyPairCompareLess<long, SRecordInfo>
    bool operator()(const SKeyedRecord& a, const SKeyedRecord& b) const { return a.first < b.first; }
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SKeyedRecord* __fastcall __copy_backward(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord* result,
                                         const SRandomAccessIteratorTag& category, long* distance);   // 0x0071EDD0
void __fastcall __unguarded_linear_insert(SKeyedRecord* last, SKeyedRecord value, SKeyLess comp);   // 0x0071F5E0

static inline SKeyedRecord* copy_backward(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord* result)
{
    return __copy_backward(first, last, result, SRandomAccessIteratorTag(), (long*)0);
}

void __fastcall __linear_insert(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord value, SKeyLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}
// ---- end source ----
SKeyedRecord* __fastcall __copy_backward(SKeyedRecord* first, SKeyedRecord* last, SKeyedRecord* result,
                                         const SRandomAccessIteratorTag&, long*)
{
    while (last != first) { --last; --result; *result = *last; }
    return result;
}
void __fastcall __unguarded_linear_insert(SKeyedRecord* last, SKeyedRecord value, SKeyLess comp)
{
    SKeyedRecord* next = last - 1;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}
static SKeyedRecord Make(long k, long v)
{
    long raw[6]; raw[0] = k; raw[1] = v; raw[2] = v + 1; raw[3] = v + 2; raw[4] = v + 3; raw[5] = v + 4;
    return *(const SKeyedRecord*)raw;
}
int main()
{
    SKeyedRecord a[4] = { Make(5, 50), Make(7, 70), Make(9, 90), Make(0, 0) };
    __linear_insert(a, a + 3, Make(1, 10), SKeyLess());      // goes to the front
    CHECK(a[0].first == 1 && a[0].second.value == 10 && a[0].second.tail.m_00 == 11 && a[0].second.tail.m_04 == 12);
    CHECK(*(long*)a[0].second.tail.m_bytes == 13 && *(long*)(a[0].second.tail.m_bytes + 4) == 14);
    CHECK(a[1].first == 5 && a[2].first == 7 && a[3].first == 9 && a[3].second.tail.m_00 == 91);
    SKeyedRecord b[4] = { Make(2, 20), Make(4, 40), Make(8, 80), Make(6, 60) };
    __linear_insert(b, b + 3, b[3], SKeyLess());              // unguarded insert of 6 from slot 3
    CHECK(b[0].first == 2 && b[1].first == 4 && b[2].first == 6 && b[3].first == 8);
    CHECK(b[2].second.value == 60 && b[3].second.value == 80 && b[2].second.tail.m_04 == 62);
    SKeyedRecord c[3] = { Make(-3, 1), Make(-1, 2), Make(-2, 3) };   // signed keys
    __linear_insert(c, c + 2, c[2], SKeyLess());
    CHECK(c[0].first == -3 && c[1].first == -2 && c[2].first == -1);
    if (fails == 0) printf("00720cb0_TEST PASS\n"); else printf("00720cb0_TEST FAIL\n");
    return fails;
}