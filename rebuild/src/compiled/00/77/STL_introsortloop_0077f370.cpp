// std::__introsort_loop(SNamedPosition* first, SNamedPosition* last, SNamedPosition*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort recursion over 16-byte (CCharString, C3DVector) pairs ordered by name: median-of-three quicksort partitions down to 16 elements, heap sort once the depth budget is spent
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F370, 200 bytes (+ int3 padding to 208), `ret 0Ch` (__fastcall /Gr
//   algorithm TU: first in ecx -> ebx, last in edx -> edi, then the null value-type pointer, the depth budget and the
//   empty comparator on the stack).
// Callees: 0x0077E820 __median(a, b, c, comp) (out of line, returns a const reference; catalogued as the propagated
//   `_Med3<pair<CCharString,CCharString>*,...>`), 0x0099EC30 CCharString copy constructor (the by-value pivot
//   `_Tp(__median(...))` built in the argument area by the pair's memberwise copy constructor), 0x0077E8F0
//   __unguarded_partition, itself (recursion on the right part), 0x0077F2C0 __partial_sort (partial_sort(first, last,
//   last, comp) inlined to its core).
// Instantiation: sort<pair<CCharString, C3DVector>*, CKeyPairCompareLess<CCharString, C3DVector>>.
// Shape: STLport `while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last, last,
//   comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first, *(first + (last -
//   first)/2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp); last = cut; }` --
//   the size test is `and eax,-16 ; cmp eax,100h`, the decremented budget is stored back to its argument slot.
// Label correction: catalogued as `Sort_QuicksortPhase_CCharString`; the body is STLport `__introsort_loop`.
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

const SNamedPosition& __fastcall __median(const SNamedPosition& a, const SNamedPosition& b, const SNamedPosition& c,
                                          CNamedPositionLess comp);   // 0x0077E820
SNamedPosition* __fastcall __unguarded_partition(SNamedPosition* first, SNamedPosition* last, SNamedPosition pivot,
                                                 CNamedPositionLess comp);   // 0x0077E8F0
void __fastcall __partial_sort(SNamedPosition* first, SNamedPosition* middle, SNamedPosition* last, SNamedPosition*,
                               CNamedPositionLess comp);   // 0x0077F2C0

inline void partial_sort(SNamedPosition* first, SNamedPosition* middle, SNamedPosition* last, CNamedPositionLess comp)
{
    __partial_sort(first, middle, last, (SNamedPosition*)0, comp);
}

void __fastcall __introsort_loop(SNamedPosition* first, SNamedPosition* last, SNamedPosition*, int depth_limit,
                                 CNamedPositionLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        SNamedPosition* cut = __unguarded_partition(first, last,
                                                    SNamedPosition(__median(*first, *(first + (last - first) / 2),
                                                                            *(last - 1), comp)),
                                                    comp);
        __introsort_loop(cut, last, (SNamedPosition*)0, depth_limit, comp);
        last = cut;
    }
}