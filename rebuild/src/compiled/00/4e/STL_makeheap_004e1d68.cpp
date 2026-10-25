// std::__make_heap(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp, SRegisteredTCEntry*, int*) -- STLport heap build over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, ordered by name): sift every parent down from the last one to the root with __adjust_heap
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1D68, 75 bytes, `ret 0Ch` (size-optimised TU: unaligned entry).
// Callees: the pair copy constructor 0x004D788D (thiscall, out of line in this /Os TU), __adjust_heap(first, parent,
//   len, val, comp) 0x004DFDBB (`__fastcall`; catalogued as `_Adjust_heap<pair<CCharString,CRegisteredTCInfo>*,...>`,
//   which names the element).
// Label correction: the catalog's `Heap_SiftDown` is descriptive; the body is STLport's `__make_heap` for the 16-byte
//   registry pair (see 0x004DFD60 in this TU).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx; the empty comparator, the null `_Tp*`
//   and `_Distance*` tags on the stack (`ret 0Ch`).
// Shape: the STLport text (`if (last - first < 2) return; len = last - first; parent = (len - 2) / 2;
//   while (true) { __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return;
//   parent--; }`) under `optimize("s")`: the comparator is passed through with `push dword ptr [esp+14h]`, the
//   by-value copy is built in the argument area by the out-of-line pair copy constructor, `first + parent` is
//   strength-reduced into edi.
#pragma optimize("s", on)
namespace std {

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

void __fastcall __adjust_heap(SRegisteredTCEntry* first, int holeIndex, int len, SRegisteredTCEntry val,
                              CKeyPairCompareLess comp);   // 0x004DFDBB

void __fastcall __make_heap(SRegisteredTCEntry* first, SRegisteredTCEntry* last, CKeyPairCompareLess comp,
                            SRegisteredTCEntry*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;

    while (true) {
        __adjust_heap(first, parent, len, SRegisteredTCEntry(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

}