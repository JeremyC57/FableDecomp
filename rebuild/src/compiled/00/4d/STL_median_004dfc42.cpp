// std::__median(const pair<CCharString, CCharString>& a, const pair<CCharString, CCharString>& b, const pair<CCharString, CCharString>& c, CKeyPairCompareLess<CCharString, CCharString> comp) -- SGI median-of-three for the introsort pivot of a string-pair array ordered by the first string (inline CCharString::operator<)
// Leaf: __median
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFC42, 168 bytes, `ret 8` (__fastcall: a in ecx, b in edx, c and the
// empty comparator's slot on the stack -- the SGI/STLport algorithm TUs are /Gr); the chosen element's address in eax.
// Callee: CBasicString<char>::operator< @ 0x00429950 (the string-data compare behind the inline CCharString::operator<;
// the landed 0x00AF3000 model). Single caller 0x004E827B (the introsort loop).
// Shape: STLport `if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; } else if
// (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;` with CKeyPairCompareLess comparing `.first`
// only; a and b are spilled to the ebp frame for the returns. /Oa (assume no aliasing): the a/b/c string-data pointers
// are loaded once and stay in edi/esi/ebx across the data-compare calls (the same body as 0x00A70020 of chunk 356).
// TU pragmas: this 0x4DFxxx TU keeps the frame pointer and is size-optimised (`push ebp ; mov ebp,esp ; push ecx ; push
// ecx`, the shared `jmp` to one `leave` epilogue): `#pragma optimize("y", off)` + `#pragma optimize("s", on)`; under
// "s" VC7.1 calls the comparator and CCharString::operator< out of line unless they are __forceinline.
// Label: catalogued as Dinkumware `_Med3<pair<CCharString,CCharString>*, CKeyPairCompareLess<CCharString,CCharString> >`;
// the body returns a reference and never swaps, i.e. SGI's `__median`.
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};

template <class K, class V>
struct CKeyPairCompareLess {                       // empty functor, passed in a 4-byte slot
    __forceinline bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};

typedef pair<CCharString, CCharString> CStringPair;
typedef CKeyPairCompareLess<CCharString, CCharString> CStringPairLess;

const CStringPair& __fastcall __median(const CStringPair& a, const CStringPair& b, const CStringPair& c, CStringPairLess comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}