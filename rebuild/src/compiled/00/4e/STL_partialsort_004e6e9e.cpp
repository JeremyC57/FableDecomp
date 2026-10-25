// std::__partial_sort(SRegisteredTCEntry* first, SRegisteredTCEntry* middle, SRegisteredTCEntry* last, SRegisteredTCEntry*, CKeyPairCompareLess comp) -- STLport partial sort over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, ordered by name): heap the first part, pop every smaller element of the tail into it, then sort the heap
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6E9E, 104 bytes, `ret 0Ch` (size-optimised TU: unaligned entry,
//   frame pointer).
// Callees: make_heap(first, last, comp) 0x004E36C3 and sort_heap(first, last, comp) 0x004E41DD (`__fastcall`, the
//   inline STLport front ends out of line in this /Os TU; 0x004E41DD is catalogued as `_Sort_heap<...>`),
//   CBasicString<char>::operator< 0x00429950 (the inlined CCharString::operator<), the pair copy constructor
//   0x004D788D, __pop_heap(first, last, result, val, comp, ptrdiff_t*) 0x004E1D20 (`__fastcall`).
// Label correction: the catalog's `_Find<CCharString*,CCharString>` is propagated; the body is STLport's
//   `__partial_sort` (it is the introsort loop's depth-limit fallback for the 16-byte registry pair, see 0x004DFD60).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, middle in edx; last, the null `_Tp*` tag and the
//   empty comparator on the stack (`ret 0Ch`).
// Shape: the STLport text `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` under
//   `optimize("s")` with the comparator and CCharString::operator< `__forceinline` (the landed 0x00AF2F10 inline);
//   the by-value copy is built in the argument area by the out-of-line pair copy constructor, the distance tag is a
//   null `push 0`; `*first`'s block pointer is re-read every pass (the heap top changes in __pop_heap).
#pragma optimize("s", on)
namespace std {

struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    __forceinline bool operator<(const CCharString& other) const
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

struct CThing;
struct CTCBase;

struct CRegisteredTCInfo {                                 // CTCBase::CRegisteredTCInfo (12 bytes, Ego_r.pdb)
    CTCBase* (__cdecl* AllocFunc)(CThing*);                // +0x00
    unsigned long DriverType;                              // +0x04
    CCharString   Name;                                    // +0x08
};

template <class T1, class T2> struct pair {                // STLport pair
    T1 first;
    T2 second;
    pair(const pair& p);                                   // 0x004D788D (out of line in this TU)
};
typedef pair<CCharString, CRegisteredTCInfo> SRegisteredTCEntry;   // 16 bytes

struct CKeyPairCompareLess {                               // CKeyPairCompareLess<CCharString, CRegisteredTCInfo>
    __forceinline bool operator()(const SRegisteredTCEntry& a, const SRegisteredTCEntry& b) const { return a.first < b.first; }
};

void __fastcall make_heap(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp);   // 0x004E36C3
void __fastcall sort_heap(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp);   // 0x004E41DD
void __fastcall __pop_heap(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry* result,
                           SRegisteredTCEntry val, CKeyPairCompareLess comp, int*);   // 0x004E1D20

void __fastcall __partial_sort(SRegisteredTCEntry* first, SRegisteredTCEntry* middle, SRegisteredTCEntry* last,
                               SRegisteredTCEntry*, CKeyPairCompareLess comp)
{
    make_heap(first, middle, comp);
    for (SRegisteredTCEntry* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SRegisteredTCEntry(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

}