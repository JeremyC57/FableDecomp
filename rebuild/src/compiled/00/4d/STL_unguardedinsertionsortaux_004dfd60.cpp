// _STL::__unguarded_insertion_sort_aux(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry*, CKeyPairCompareLess comp) -- STLport unguarded insertion sort over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, ordered by name): every element from first to last is copied out and sifted down by the out-of-line __unguarded_linear_insert
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFD60, 42 bytes, `ret 8` (size-optimised TU: unaligned entry).
// Callees: the pair copy constructor 0x004D788D (thiscall, out of line in this /Os TU; catalogued as
//   `pair<CCharString,CCharString>`), __unguarded_linear_insert(last, val, comp) 0x004DD80B (`__fastcall`, the pair by
//   value and the comparator on the stack; catalogued as `std::set_Insert_Sorted`).
// Label correction: the catalog's `_Fill<CIntelligentPointer<CThing>*,...>` is propagated; the element is the 16-byte
//   pair<CCharString, CRegisteredTCInfo> of the registry CVectorMap (Ego_r.pdb CRegisteredTCInfo: AllocFunc +0,
//   DriverType +4, Name +8), the same element as 0x004DFCEA / 0x004E1C8A / 0x004E1D68 in this TU.
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the null `_Tp*` tag and the empty
//   comparator on the stack.
// Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` under `optimize("s")`: the
//   comparator is passed through with `push dword ptr [esp+10h]`, the by-value copy is built in the argument area by
//   the out-of-line pair copy constructor.
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

void __fastcall __unguarded_linear_insert(SRegisteredTCEntry* last, SRegisteredTCEntry val, CKeyPairCompareLess comp);   // 0x004DD80B

void __fastcall __unguarded_insertion_sort_aux(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry*,
                                               CKeyPairCompareLess comp)
{
    for (SRegisteredTCEntry* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SRegisteredTCEntry(*i), comp);
}

}