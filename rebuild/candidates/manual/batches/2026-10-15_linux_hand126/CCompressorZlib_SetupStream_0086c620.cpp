// CCompressorZlib::SetupStream(SElement16* first, SElement16* last, SElement16 value, SKeyLess comp) -- (label propagated; really the STLport sort helper __linear_insert for 16-byte records keyed by a signed first word) shift [first, last) up one slot and put value at first when it sorts before *first, else run the unguarded insert from last
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086C620, 107 bytes.
// Callees: __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) 0x0086A4A0,
//          __unguarded_linear_insert(last, value, comp) 0x0086A6E0.
// The twin of 0x0076AAE0 for a record {long key; 12-byte POD value}: the key compare is signed (`jge`)
// and the element's copy constructor copies the POD member as a unit at +4 (`add eax,4`).
//
// Shape: the manifest's CCompressorZlib::SetupStream label is propagated over byte-identical bodies;
// the code is STLport `__linear_insert(first, last, val, comp)` from a /Gr TU (GOTCHAS: SGI/STLport
// algorithm templates are __fastcall: first in ecx, last in edx, the 16-byte value and the 4-byte
// empty comparator on the stack, `ret 14h`). The comparator's operator() is inlined (`cmp edi,[esi] ;
// jge` = signed key less-than); copy_backward reaches __copy_backward with the iterator tag bound to
// a const reference -- the empty temporary is homed in the comparator's dead argument slot (`lea
// eax,[esp+20h]`) -- and a null distance pointer; the record has a user memberwise copy constructor,
// so the by-value value for __unguarded_linear_insert is built in the argument area (`sub esp,10h ;
// mov eax,esp`) after the comparator push.
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct SValue12 {
    long m_value0;                          // +0x00
    long m_value1;                          // +0x04
    long m_value2;                          // +0x08
};

struct SElement16 {
    long m_key;                             // +0x00 sort key
    SValue12 m_value;                       // +0x04
    SElement16(const SElement16& o) : m_key(o.m_key), m_value(o.m_value) { }
};

struct SKeyLess {                           // empty comparator, passed by value (4-byte slot)
    bool operator()(const SElement16& a, const SElement16& b) const { return a.m_key < b.m_key; }
};

SElement16* __fastcall __copy_backward(SElement16* first, SElement16* last, SElement16* result,
                                       const random_access_iterator_tag& tag, int* distance);   // 0x0086A4A0
void __fastcall __unguarded_linear_insert(SElement16* last, SElement16 value, SKeyLess comp);   // 0x0086A6E0

void __fastcall __linear_insert(SElement16* first, SElement16* last, SElement16 value, SKeyLess comp)
{
    if (comp(value, *first)) {
        __copy_backward(first, last, last + 1, random_access_iterator_tag(), (int*)0);
        *first = value;
    } else {
        __unguarded_linear_insert(last, value, comp);
    }
}
