// _STL::__introsort_loop(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry*, int depth_limit, CKeyPairCompareLess comp) -- STLport introsort core over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, ordered by name): while more than 16 elements remain, partition around the median of three and recurse on the right part, falling back to partial_sort when the depth budget runs out
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E823B, 138 bytes, `ret 0Ch` (size-optimised TU: unaligned entry,
//   frame pointer).
// Callees: partial_sort(first, middle, last, comp) 0x004E77B6 (`__fastcall`, the inline STLport front end out of line
//   in this /Os TU), __median(a, b, c, comp) 0x004DFC42 (`__fastcall`, returns `const T&`), the pair copy constructor
//   0x004D788D, __unguarded_partition 0x004DFCEA (this chunk), itself (recursion on [cut, last)).
// Label correction: the catalog's `Partition_WithMedianOfThree` is descriptive; the body is STLport's
//   `__introsort_loop` for the 16-byte registry pair (see 0x004DFD60).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx; the null `_Tp*` tag, depth_limit and
//   the empty comparator on the stack (`ret 0Ch`).
// Shape: the STLport text (`while (last - first > __stl_threshold) { if (depth_limit == 0) { partial_sort(first, last,
//   last, comp); return; } --depth_limit; cut = __unguarded_partition(first, last, _Tp(__median(*first,
//   *(first + (last - first)/2), *(last - 1), comp)), comp); __introsort_loop(cut, last, (_Tp*)0, depth_limit, comp);
//   last = cut; }`) under `optimize("s")`: both arms' first push (the comparator) and `mov ecx,first` are hoisted above
//   the depth test, depth_limit is decremented in its argument slot, the pivot is built in the argument area by the
//   out-of-line pair copy constructor from __median's reference; `(last - first) > 16` folds to `and ; cmp 100h`.
#pragma optimize("s", on)
namespace _STL {

struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
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

struct CKeyPairCompareLess { };                            // CKeyPairCompareLess<CCharString, CRegisteredTCInfo>

enum { __stl_threshold = 16 };

void __fastcall partial_sort(SRegisteredTCEntry* first, SRegisteredTCEntry* middle, SRegisteredTCEntry* last,
                             CKeyPairCompareLess comp);   // 0x004E77B6
const SRegisteredTCEntry& __fastcall __median(const SRegisteredTCEntry& a, const SRegisteredTCEntry& b,
                                              const SRegisteredTCEntry& c, CKeyPairCompareLess comp);   // 0x004DFC42
SRegisteredTCEntry* __fastcall __unguarded_partition(SRegisteredTCEntry* first, SRegisteredTCEntry* last,
                                                     SRegisteredTCEntry pivot, CKeyPairCompareLess comp);   // 0x004DFCEA

void __fastcall __introsort_loop(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry*,
                                 int depth_limit, CKeyPairCompareLess comp)
{
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        SRegisteredTCEntry* cut =
            __unguarded_partition(first, last,
                                  SRegisteredTCEntry(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                  comp);
        __introsort_loop(cut, last, (SRegisteredTCEntry*)0, depth_limit, comp);
        last = cut;
    }
}

}