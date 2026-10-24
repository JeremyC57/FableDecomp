// _STL::__unguarded_partition(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry pivot, CKeyPairCompareLess comp) -- STLport quicksort partition step over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, ordered by name): advance first past smaller names, retreat last past larger ones, swap the out-of-place pair, return the split point; the by-value pivot is destroyed here
// Grade: DIFFER(110v113) -- ours (size TU + __forceinline comparator) builds an ebp frame and keeps the pivot string pointer in its argument slot (`cmp [ebp+8],ecx`, re-read every pass); retail has no frame (pivot.p in esi, `mov ecx,esi` before the right-scan call, re-read only after iter_swap) -- best of 4 grades (plain /O2 128v113, "s" 84v113, "s"+forceinline 110v113, +"y" on 110v113)
// Leaf: __unguarded_partition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFCEA, 113 bytes, `ret 14h` (size-optimised TU: unaligned entry).
// Caller: 0x004E828F (the introsort loop of the registry's CVectorMap Sort, CVectorMap<CCharString,
//   CTCBase::CRegisteredTCInfo,...>::Sort is named in Ego_r.pdb).
// Callees: CBasicString<char>::operator< 0x00429950 (the inlined CCharString::operator< behind its block-pointer
//   tests), iter_swap(pair*, pair*) 0x004DA6FC (`__fastcall`, out of line in this /Os TU: copy, two operator=, the
//   temporary's destructors; catalogued as `swap<CTCBase::CRegisteredTCInfo>`), ~CCharString 0x0099EAE0 (x2: the pivot's
//   second.Name at +0xc, then its key).
// Label correction: the catalog's `Partition_Step` is descriptive; the element is the 16-byte
//   pair<CCharString, CRegisteredTCInfo> (Ego_r.pdb CRegisteredTCInfo: AllocFunc +0, DriverType +4, Name +8; the landed
//   CRegisteredTCInfo constructor 0x004D2EF0 sits in this TU).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the 16-byte pivot and the empty
//   comparator on the stack (16 + 4 = 0x14).
// Shape: the STLport text `for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last)) --last;
//   if (!(first < last)) return first; iter_swap(first, last); ++first; }`; CCharString::operator< is the landed
//   0x00AF2F10 inline (same block -> false, null this -> true, null other -> false, else the payload compare); the
//   pivot's block pointer is re-read after each iter_swap call (the pivot is address-taken for its destructors).
#pragma optimize("s", on)
namespace _STL {

struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
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
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<CCharString, CRegisteredTCInfo> SRegisteredTCEntry;   // 16 bytes

struct CKeyPairCompareLess {                               // CKeyPairCompareLess<CCharString, CRegisteredTCInfo>
    __forceinline bool operator()(const SRegisteredTCEntry& a, const SRegisteredTCEntry& b) const { return a.first < b.first; }
};

void __fastcall iter_swap(SRegisteredTCEntry* a, SRegisteredTCEntry* b);   // 0x004DA6FC

SRegisteredTCEntry* __fastcall __unguarded_partition(SRegisteredTCEntry* first, SRegisteredTCEntry* last,
                                                     SRegisteredTCEntry pivot, CKeyPairCompareLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

}
