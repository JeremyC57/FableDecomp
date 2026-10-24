// _STL::__uninitialized_fill_n(pair<unsigned long, C3DHelperPoint>* first, unsigned int n, const pair<unsigned long, C3DHelperPoint>& x, const __false_type&) -- copy-construct n copies of x into raw storage at first, return the end of the copies
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A32020, 61 bytes (+ int3 padding), `ret 8` (__fastcall: first in ecx, n in
//   edx, x and the tag reference on the stack). No callees.
// Identity: the vector<pair<unsigned long, C3DHelperPoint> > fill helper called by its _M_insert_overflow 0x00A33D90; the
//   catalog's "fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>" is propagated. Element: the 20-byte helper-point
//   table entry of C3DMeshStats (the family of __unguarded_linear_insert 0x00A31280 / __linear_insert 0x00A33780).
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` with an unsigned size (`jbe`), placement
//   new's null test, and the pair's memberwise copy (key + four words; STLport's explicit pair copy constructor over a
//   second with a memberwise user copy constructor, the landed 0x00A33780 model) through a strength-reduced second
//   pointer (`lea esi,[eax+0Ch]`).
inline void* operator new(unsigned int, void* where) { return where; }

namespace _STL {

struct SRecordInfo {                                   // C3DHelperPoint as words (16 bytes)
    unsigned long m_a, m_b, m_c, m_d;
    SRecordInfo(const SRecordInfo& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c), m_d(o.m_d) { }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, SRecordInfo> TEntry;       // 20 bytes

struct __false_type { __false_type() { } };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

TEntry* __fastcall __uninitialized_fill_n(TEntry* first, unsigned int n, const TEntry& x, const __false_type&)
{
    TEntry* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

}