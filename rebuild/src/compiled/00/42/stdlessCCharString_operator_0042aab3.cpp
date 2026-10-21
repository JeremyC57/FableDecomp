// std::less<CCharString>::operator()(const CCharString&, const CCharString&) const -- string ordering for the CCharString maps
// Leaf: ??R?$less@VCCharString
// Retail: Fable.exe @ 0x0042AAB3, 41 bytes, `ret 8`. The inlined CCharString::operator<: equal data pointers -> false,
// null left -> true, null right -> false, else CBasicString<char>::operator< (0x00429950) on the two data objects. Callers
// are the map<CCharString, ...> tree code around 0x0042AD4D..0x0042BD71.
// Layout: CCharString is one pointer to its CBasicString<char> data (no engine/ header).
// Opt: size TU (unaligned entry). Shape: CCharString::operator< is __forceinline (the size TU otherwise calls it) and
// tests `other.PData == PData` -- the reversed operands are what load the right-hand string first.
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
namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template struct less<CCharString>;
}