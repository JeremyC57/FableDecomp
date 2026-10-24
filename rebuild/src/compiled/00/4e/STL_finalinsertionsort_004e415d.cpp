// _STL::__final_insertion_sort(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp) -- STLport introsort finish over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, ordered by name): a guarded insertion sort of the first 16 elements, then the unguarded insertion sort of the rest (or one guarded sort when the range is short)
// Leaf: __final_insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E415D, 68 bytes, `ret 4` (size-optimised TU: unaligned entry).
// Callees: __insertion_sort(first, last, comp) 0x004E3686 (`__fastcall`, `ret 4`), __unguarded_insertion_sort_aux(first,
//   last, _Tp*, comp) 0x004DFD60 (`__fastcall`, landed in this chunk).
// Label correction: the catalog's `make_heap<CThing**,CThingCompare_BestForTargeting2>` is propagated; the body is
//   STLport's `__final_insertion_sort` for the 16-byte registry pair (the 0x100-byte threshold = 16 elements of 16
//   bytes; 0x004DFD60 names the element).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the empty comparator on the stack.
// Shape: `if (last - first > __stl_threshold) { __insertion_sort(first, first + 16, comp);
//   __unguarded_insertion_sort(first + 16, last, comp); } else __insertion_sort(first, last, comp);` with the inline
//   `__unguarded_insertion_sort` forwarding to the aux with a null value-type tag; `(last - first) > 16` folds to
//   `and eax,-16 ; cmp eax,100h`; `optimize("s")` passes the comparator through with `push dword ptr [esp+N]`.
#pragma optimize("s", on)
namespace _STL {

struct CCharString {
    void* PStringData;
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
};
typedef pair<CCharString, CRegisteredTCInfo> SRegisteredTCEntry;   // 16 bytes

struct CKeyPairCompareLess { };                            // CKeyPairCompareLess<CCharString, CRegisteredTCInfo>

enum { __stl_threshold = 16 };

void __fastcall __insertion_sort(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp);   // 0x004E3686
void __fastcall __unguarded_insertion_sort_aux(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry*,
                                               CKeyPairCompareLess comp);   // 0x004DFD60

inline void __unguarded_insertion_sort(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp)
{
    __unguarded_insertion_sort_aux(first, last, (SRegisteredTCEntry*)0, comp);
}

void __fastcall __final_insertion_sort(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp)
{
    if (last - first > __stl_threshold) {
        __insertion_sort(first, first + __stl_threshold, comp);
        __unguarded_insertion_sort(first + __stl_threshold, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}

}