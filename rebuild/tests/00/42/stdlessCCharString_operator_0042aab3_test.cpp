// test for 0042aab3 less<CCharString>
#include <stdio.h>
#include <string.h>
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

struct Str : CBasicString<char> { const char* s; };
template <> bool CBasicString<char>::operator<(const CBasicString<char>& o) const { return strcmp(((const Str*)this)->s, ((const Str&)o).s) < 0; }
int main()
{
    Str sa, sb; sa.s = "apple"; sb.s = "banana";
    CCharString a, b, n; a.PData = &sa; b.PData = &sb; n.PData = 0;
    std::less<CCharString> lt;
    int ok = lt(a, b) == true && lt(b, a) == false && lt(a, a) == false && lt(n, a) == true && lt(a, n) == false && lt(n, n) == false;
    printf(ok ? "0042aab3_TEST PASS\n" : "0042aab3_TEST FAIL\n");
    return ok ? 0 : 1;
}