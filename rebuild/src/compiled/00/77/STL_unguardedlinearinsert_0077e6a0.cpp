// _STL::__unguarded_linear_insert(SNamedPosition* last, SNamedPosition val, CKeyPairCompareLess comp) -- STLport insertion step over 16-byte (CCharString, C3DVector) pairs ordered by name: shift greater entries up one slot until val's place, then store it
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E6A0, 133 bytes (+ int3 padding to 144), `ret 14h` (__fastcall /Gr
//   algorithm TU: last in ecx -> edi, the 16-byte by-value pair and the empty comparator on the stack, 0x10 + 4; the
//   by-value pair's string is destroyed here).
// Callees: 0x00429950 CBasicString<char>::operator< (the inlined CCharString::operator<), 0x0099EFB0
//   CCharString::operator=, 0x0099EAE0 ~CCharString.
// Instantiation: sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector>> -- the same
//   instantiation as the landed __adjust_heap 0x0077EA60, __push_heap 0x0077E730, __pop_heap 0x0077ED20 and
//   _Sort_heap 0x0077F190; called by __linear_insert 0x0077E960.
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`;
//   `next` lives in esi as `last - 1` and its vector half in ebx (`lea ebx,[esi+4]`, both stepped by 0x10); the pair's
//   IMPLICIT assignment is CCharString::operator= plus a struct copy of the C3DVector (re-based through `add edi,4`),
//   the final `*last = val` copies the vector's three words from the argument slots. CCharString::operator< compares
//   val (this) with *next (other): same payload -> false, null this -> true, null other -> false, else the payload
//   compare. The comparator's real body is compiled above the caller (auto_inline off) as in the landed 0x005E0710.
// Label correction: catalogued as `SortedMap_InsertOrdered`; the body is STLport `__unguarded_linear_insert`.
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

void __fastcall __unguarded_linear_insert(SNamedPosition* last, SNamedPosition val, CNamedPositionLess comp)
{
    SNamedPosition* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}