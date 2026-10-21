// Behaviour test for 0076aae0: src/0076aae0.cpp copied verbatim; __linear_insert shifts [first,last) up and puts the
// value at first when its key sorts before *first, else walks back from last (unguarded insert).
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// __linear_insert(SElement16* first, SElement16* last, SElement16 value, SKeyLess comp) -- STLport sort helper for 16-byte records keyed by an unsigned first word (the chunk label CCompressorZlib::SetupStream is propagated over byte-identical bodies): shift [first, last) up one slot and put value at first when it sorts before *first, else run the unguarded insert from last
// Retry 10b: the record is a key word plus a 12-byte payload struct with a user memberwise copy constructor (implicit outer copy);
// a flat four-member record with its own copy constructor sinks the key store below the other three (the parked shape).
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076AAE0, 105 bytes.
// Callees: __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) 0x00768B30,
//          __unguarded_linear_insert(last, value, comp) 0x00769420.
//
// Shape: the manifest's CCompressorZlib::SetupStream label is propagated over byte-identical bodies;
// the code is STLport `__linear_insert(first, last, val, comp)` from a /Gr TU (GOTCHAS: SGI/STLport
// algorithm templates are __fastcall: first in ecx, last in edx, the 16-byte value and the 4-byte
// empty comparator on the stack, `ret 14h`). The comparator's operator() is inlined (`cmp edi,[esi] ;
// jae` = unsigned key less-than); copy_backward reaches __copy_backward with the iterator tag bound to
// a const reference -- the empty temporary is homed in the comparator's dead argument slot (`lea
// eax,[esp+20h]`) -- and a null distance pointer; the record has a user memberwise copy constructor,
// so the by-value value for __unguarded_linear_insert is built in the argument area (`sub esp,10h ;
// mov eax,esp`) after the comparator push.
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct SPayload12 {                         // 12-byte payload with a user memberwise copy constructor
    long m_value0;                          // +0x00
    long m_value1;                          // +0x04
    long m_value2;                          // +0x08
    SPayload12(const SPayload12& o) : m_value0(o.m_value0), m_value1(o.m_value1), m_value2(o.m_value2) { }
};

struct SElement16 {
    unsigned long m_key;                    // +0x00 sort key
    SPayload12 m_payload;                   // +0x04
};

struct SKeyLess {                           // empty comparator, passed by value (4-byte slot)
    bool operator()(const SElement16& a, const SElement16& b) const { return a.m_key < b.m_key; }
};

SElement16* __fastcall __copy_backward(SElement16* first, SElement16* last, SElement16* result,
                                       const random_access_iterator_tag& tag, int* distance);   // 0x00768B30
void __fastcall __unguarded_linear_insert(SElement16* last, SElement16 value, SKeyLess comp);   // 0x00769420

void __fastcall __linear_insert(SElement16* first, SElement16* last, SElement16 value, SKeyLess comp)
{
    if (comp(value, *first)) {
        __copy_backward(first, last, last + 1, random_access_iterator_tag(), (int*)0);
        *first = value;
    } else {
        __unguarded_linear_insert(last, value, comp);
    }
}
// ---- end source ----
SElement16* __fastcall __copy_backward(SElement16* first, SElement16* last, SElement16* result,
                                       const random_access_iterator_tag&, int*)
{
    while (last != first) { --last; --result; *result = *last; }
    return result;
}
void __fastcall __unguarded_linear_insert(SElement16* last, SElement16 value, SKeyLess comp)
{
    SElement16* next = last - 1;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}
static SElement16 Make(unsigned long k, long v)
{
    long raw[4]; raw[0] = (long)k; raw[1] = v; raw[2] = v + 1; raw[3] = v + 2;   // no default ctor (payload has a user copy ctor)
    return *(const SElement16*)raw;
}
int main()
{
    SElement16 a[4] = { Make(5, 50), Make(7, 70), Make(9, 90), Make(0, 0) };
    __linear_insert(a, a + 3, Make(1, 10), SKeyLess());      // goes to the front
    CHECK(a[0].m_key == 1 && a[0].m_payload.m_value2 == 12);
    CHECK(a[1].m_key == 5 && a[2].m_key == 7 && a[3].m_key == 9 && a[3].m_payload.m_value1 == 91);
    SElement16 b[4] = { Make(2, 20), Make(4, 40), Make(8, 80), Make(6, 60) };
    __linear_insert(b, b + 3, b[3], SKeyLess());              // unguarded insert of 6 from slot 3
    CHECK(b[0].m_key == 2 && b[1].m_key == 4 && b[2].m_key == 6 && b[3].m_key == 8);
    CHECK(b[2].m_payload.m_value0 == 60 && b[3].m_payload.m_value0 == 80);
    if (fails == 0) printf("0076aae0_TEST PASS\n"); else printf("0076aae0_TEST FAIL\n");
    return fails;
}