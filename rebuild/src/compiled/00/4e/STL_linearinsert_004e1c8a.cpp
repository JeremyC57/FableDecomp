// std::__linear_insert(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry val, CKeyPairCompareLess comp) -- STLport insertion-sort step over the thing-component registry (pair<CCharString, CTCBase::CRegisteredTCInfo>, ordered by name): a value whose name sorts before the first element shifts [first, last) up one slot and is assigned to *first, anything else goes through the unguarded insert; the by-value pair is destroyed here
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1C8A, 114 bytes, `ret 14h` (size-optimised TU: unaligned entry,
//   frame pointer).
// Callees: CBasicString<char>::operator< 0x00429950 (the inlined CCharString::operator<), __copy_backward(first, last,
//   result, const random_access_iterator_tag&, ptrdiff_t*) 0x004DA738 (`__fastcall`; catalogued as the propagated
//   `_Copy_backward<CArray<unsigned_short>*,...>`), the pair operator= 0x004DA56A and copy constructor 0x004D788D
//   (thiscall, out of line in this /Os TU), __unguarded_linear_insert(last, val, comp) 0x004DD80B (`__fastcall`),
//   ~CCharString 0x0099EAE0 (x2: the value's second.Name at +0xc, then its key).
// Label correction: the catalog's `Set_Insert_Element` is descriptive; the element is the 16-byte
//   pair<CCharString, CRegisteredTCInfo> of the registry CVectorMap (see 0x004DFD60 in this TU).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the 16-byte value and the empty
//   comparator on the stack (16 + 4 = 0x14).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` under `optimize("s")` with the comparator and CCharString::operator<
//   `__forceinline` (the landed 0x00AF2F10 inline: same block -> false, null this -> true, null other -> false, else
//   the payload compare); copy_backward's fresh `random_access_iterator_tag()` lands in the high byte of the
//   comparator's argument slot (`lea eax,[ebp+1Bh]`), the distance pointer is a null `push 0`.
#pragma optimize("s", on)
namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

struct CBasicStringChar {
    char* PData;
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

} // (hoisted to global scope: naming wave 9)
class CCharString { public:
    std::CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
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
namespace std {

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
    pair& operator=(const pair& p);                        // 0x004DA56A (out of line in this TU)
};
typedef pair<CCharString, CRegisteredTCInfo> SRegisteredTCEntry;   // 16 bytes

struct CKeyPairCompareLess {                               // CKeyPairCompareLess<CCharString, CRegisteredTCInfo>
    __forceinline bool operator()(const SRegisteredTCEntry& a, const SRegisteredTCEntry& b) const { return a.first < b.first; }
};

SRegisteredTCEntry* __fastcall __copy_backward(SRegisteredTCEntry* first, SRegisteredTCEntry* last,
                                               SRegisteredTCEntry* result, const random_access_iterator_tag&,
                                               int*);   // 0x004DA738

__forceinline SRegisteredTCEntry* copy_backward(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __unguarded_linear_insert(SRegisteredTCEntry* last, SRegisteredTCEntry val, CKeyPairCompareLess comp);   // 0x004DD80B

void __fastcall __linear_insert(SRegisteredTCEntry* first, SRegisteredTCEntry* last, SRegisteredTCEntry val,
                                CKeyPairCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

}