#include <cstdio>
class CCharStringData { public:
    const char*   Chars;                                 // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare(const char* a, const char* b);   // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) == 0;
        return false;
    }
    bool operator!=(const CCharString& other) const;
};
bool CCharString::operator!=(const CCharString& other) const
{
    return !(*this == other);
}

#include <string.h>
long __fastcall Compare(const char* a, const char* b) { return strcmp(a, b); }
int main()
{
    CCharStringData da = { "abc", 3 }, db = { "abc", 3 }, dc = { "abd", 3 }, dd = { "ab", 2 };
    CCharString a, b, c, d, n, n2; a.PStringData = &da; b.PStringData = &db; c.PStringData = &dc; d.PStringData = &dd; n.PStringData = 0; n2.PStringData = 0;
    bool ok = !(a != a) && !(a != b) && (a != c) && (a != d) && (a != n) && (n != a) && !(n != n2);
    std::printf(ok ? "0099e900_TEST PASS\n" : "0099e900_TEST FAIL\n");
    return ok ? 0 : 1;
}