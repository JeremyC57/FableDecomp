// std::operator<(const pair<CCharString, CCharString>& x, const pair<CCharString, CCharString>& y) -- STLport lexicographic pair compare of two string pairs: first strings, then (when neither first sorts before the other) the second strings, each through the inline CCharString::operator<
// Leaf: ??$?M
// Flags: /Oa
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A40250, 110 bytes, plain `ret` (__cdecl, both references on the stack),
// result 0/1 in eax. Callee: CBasicString<char>::operator< @ 0x00429950 (the string-data compare behind the inline
// CCharString::operator<; the landed 0x00AF3000 model).
// Shape: `return x.first < y.first || (!(y.first < x.first) && x.second < y.second);` -- CCharString::operator< tests
// `other.PData == PData` (false), a null left (true), a null right (false), then calls the data compare; VC7.1 threads
// the equal-pointer exit of the first test straight to the second-string compare. /Oa (assume no aliasing): retail keeps
// both first-string data pointers in edi/esi across the data-compare call for the reversed test; without it VC7.1 reloads
// them (`mov eax,[edi] ; mov ecx,[esi]`) and the pair pointers do not need ebx/ebp.
// Label correction: catalogued as `_Med3<pair<CCharString,CCharString>*, CKeyPairCompareLess<CCharString,CCharString> >`
// (propagated over a family of bodies); this one is the out-of-line pair operator< (no third argument, cdecl).
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;
    bool operator<(const CCharString& other) const
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

template <class T1, class T2>
bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template bool operator<(const pair<CCharString, CCharString>& x, const pair<CCharString, CCharString>& y);