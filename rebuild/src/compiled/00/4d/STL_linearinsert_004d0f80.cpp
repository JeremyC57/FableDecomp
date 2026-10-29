// std::__linear_insert(SFloatStringPair* first, SFloatStringPair* last, SFloatStringPair val, CKeyPairCompareLess comp) -- STLport insertion-sort step over the CSubtitledMessage text map's 8-byte pair<float, CCharString>: a value whose key is below the first element shifts [first, last) up one slot and is assigned to *first, anything else goes through the unguarded insert; the by-value pair is destroyed here
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0F80, 120 bytes (+ int3 padding), `ret 0Ch`.
// Callees: __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) 0x004CF580 (`__fastcall`,
//   out of line for this element); CCharString::operator=(const CCharString&) 0x0099EFB0; CCharString copy constructor
//   0x0099EC30; ~CCharString 0x0099EAE0; __unguarded_linear_insert(last, val, comp) 0x004CFCF0 (`__fastcall`).
// Label correction: the catalog's `InsertionSort_InsertStringPair` is descriptive; the body is STLport's
//   `__linear_insert` for the CVectorMap<float, CCharString, CKeyPairCompareLess<...>> element (Ego_r.pdb).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the 8-byte value and the empty
//   comparator on the stack (`ret 0Ch`).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward dispatches to the out-of-line `__copy_backward`
//   with a fresh `random_access_iterator_tag()` homed in the (dead on this path) comparator slot and a null
//   distance pointer; `*first = val` is the implicit pair operator= (key dword, then CCharString::operator=); the
//   else arm copy-constructs the argument in the argument area; both arms end with the by-value parameter's
//   ~CCharString.
namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

} // (hoisted to global scope: naming wave 9)
class CCharString { public:                            // 4 bytes: the string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace std {

template <class T1, class T2> struct pair {     // STLport pair: memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<float, CCharString> SFloatStringPair;   // 8 bytes

struct CKeyPairCompareLess {                    // CKeyPairCompareLess<float, CCharString>
    bool operator()(const SFloatStringPair& a, const SFloatStringPair& b) const { return a.first < b.first; }
};

SFloatStringPair* __fastcall __copy_backward(SFloatStringPair* first, SFloatStringPair* last, SFloatStringPair* result,
                                             const random_access_iterator_tag&, int*);   // 0x004CF580

inline SFloatStringPair* copy_backward(SFloatStringPair* first, SFloatStringPair* last, SFloatStringPair* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __unguarded_linear_insert(SFloatStringPair* last, SFloatStringPair val, CKeyPairCompareLess comp);   // 0x004CFCF0

void __fastcall __linear_insert(SFloatStringPair* first, SFloatStringPair* last, SFloatStringPair val,
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