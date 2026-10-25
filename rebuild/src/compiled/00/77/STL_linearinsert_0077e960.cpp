// std::__linear_insert(SNamedPosition* first, SNamedPosition* last, SNamedPosition val, CKeyPairCompareLess comp) -- STLport insertion-sort step over 16-byte (CCharString, C3DVector) pairs ordered by name: a value that sorts before the first element shifts [first, last) up one slot and is stored at *first, anything else goes through the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E960, 166 bytes (+ int3 padding to 176), `ret 14h` (__fastcall /Gr
//   algorithm TU: first in ecx -> esi, last in edx -> edi, the 16-byte by-value pair and the empty comparator on the
//   stack; the by-value pair's string is destroyed here on both paths).
// Callees: 0x00429950 CBasicString<char>::operator< (the inlined CCharString::operator<), 0x0077E170
//   __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) (landed), 0x0099EFB0
//   CCharString::operator=, 0x0099EC30 CCharString copy constructor (the by-value pair rebuilt in the argument area),
//   0x0077E6A0 __unguarded_linear_insert(last, val, comp), 0x0099EAE0 ~CCharString.
// Instantiation: sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector>> (same TU as the
//   landed __adjust_heap 0x0077EA60).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward's fresh `random_access_iterator_tag()` is homed in
//   the comparator's dead argument slot (`lea eax,[esp+20h]`), the distance pointer is a null `push 0`; the pair's
//   memberwise user copy constructor (CCharString copy + three dword moves) builds the forwarded value.
// Label correction: catalogued as `SortedMap_InsertOrUpdate`; the body is STLport `__linear_insert`.
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

struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
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

struct C3DVector {                                         // floats, moved as dwords
    unsigned long X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

template <class T1, class T2> struct pair {                // STLport pair
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<CCharString, C3DVector> SNamedPosition;       // 16 bytes

template <class K, class V> struct CKeyPairCompareLess {   // empty, passed by value
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, C3DVector> CNamedPositionLess;

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SNamedPosition* __fastcall __copy_backward(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result,
                                           const random_access_iterator_tag&, int*);   // 0x0077E170

inline SNamedPosition* copy_backward(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __unguarded_linear_insert(SNamedPosition* last, SNamedPosition val, CNamedPositionLess comp);   // 0x0077E6A0

void __fastcall __linear_insert(SNamedPosition* first, SNamedPosition* last, SNamedPosition val, CNamedPositionLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}