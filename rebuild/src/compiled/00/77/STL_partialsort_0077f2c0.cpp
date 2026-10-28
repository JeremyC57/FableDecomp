// std::__partial_sort(SNamedPosition* first, SNamedPosition* middle, SNamedPosition* last, SNamedPosition*, CKeyPairCompareLess comp) -- STLport partial_sort core over 16-byte (CCharString, C3DVector) pairs ordered by name: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F2C0, 135 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in
//   ecx -> ebp, middle in edx -> ebx; last, the null value-type pointer and the empty comparator on the stack).
// Callees: 0x0077EDA0 __make_heap(first, middle, comp, (T*)0, (int*)0), 0x00429950 CBasicString<char>::operator<
//   (the inlined CCharString::operator<), 0x0099EC30 CCharString copy constructor (the by-value `_Tp(*i)` built in
//   the argument area), 0x0077ED20 __pop_heap(first, middle, i, val, comp, (int*)0) (landed), 0x0077F190 sort_heap
//   (landed as `_Sort_heap`, same instantiation).
// Instantiation: sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector>> (the depth-limit
//   fallback of __introsort_loop 0x0077F370).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _STLP_DISTANCE_TYPE(first, _RandomAccessIter)); sort_heap(first,
//   middle, comp);` -- the comparator word rides in edi between the calls (re-read from its slot after the pop).
// Label correction: catalogued as `_Find<CCharString*,CCharString>` (propagated); the body is STLport `__partial_sort`.
struct CBasicStringChar {                                  // CBasicString<char>
    char* PData;
    __forceinline long Compare(const CBasicStringChar& other) const
    {
        const char* b = other.PData;
        const char* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

#pragma auto_inline(off)
bool CBasicStringChar::operator<(const CBasicStringChar& other) const
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)

class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
};

class C3DVector { public:                                         // floats, moved as dwords
    unsigned long X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

template <class T1, class T2> struct pair {                // STLport pair
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<CCharString, C3DVector> SNamedPosition;       // 16 bytes

template <class K, class V> class CKeyPairCompareLess { public:   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, C3DVector> CNamedPositionLess;

void __fastcall __make_heap(SNamedPosition* first, SNamedPosition* last, CNamedPositionLess comp, SNamedPosition*,
                            int*);   // 0x0077EDA0
void __fastcall __pop_heap(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result, SNamedPosition val,
                           CNamedPositionLess comp, int*);   // 0x0077ED20
void __fastcall sort_heap(SNamedPosition* first, SNamedPosition* last, CNamedPositionLess comp);   // 0x0077F190

inline void make_heap(SNamedPosition* first, SNamedPosition* last, CNamedPositionLess comp)
{
    __make_heap(first, last, comp, (SNamedPosition*)0, (int*)0);
}

void __fastcall __partial_sort(SNamedPosition* first, SNamedPosition* middle, SNamedPosition* last, SNamedPosition*,
                               CNamedPositionLess comp)
{
    make_heap(first, middle, comp);
    for (SNamedPosition* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SNamedPosition(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}