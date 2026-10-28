// Test: CWideString::operator!= -- equal when sharing a block or holding the same characters; a null string equals only a null string
#include <cstdio>
#include <cstring>
class CWideStringData { public: const unsigned short* DataStart; long Size; };
class CWideString { public:
    CWideStringData* PStringData;
    bool Equal(const CWideString& o) const
    {
        if (PStringData == o.PStringData) return true;
        if (PStringData == 0 || o.PStringData == 0) return false;
        return PStringData->Size == o.PStringData->Size
            && std::memcmp(PStringData->DataStart, o.PStringData->DataStart, PStringData->Size * 2) == 0;
    }
    bool operator!=(const CWideString& o) const { return !Equal(o); }
};
int main()
{
    unsigned short ab[2] = { 'a', 'b' }, ab2[2] = { 'a', 'b' }, ac[2] = { 'a', 'c' };
    CWideStringData d1 = { ab, 2 }, d2 = { ab2, 2 }, d3 = { ac, 2 }, d4 = { ab, 1 };
    CWideString s1 = { &d1 }, s2 = { &d2 }, s3 = { &d3 }, s4 = { &d4 }, n1 = { 0 }, n2 = { 0 };
    if ((s1 != s1) || (s1 != s2) || !(s1 != s3) || !(s1 != s4) || (n1 != n2) || !(n1 != s1) || !(s1 != n1)) { std::printf("0099b0f0_TEST FAIL\n"); return 1; }
    std::printf("0099b0f0_TEST PASS\n");
    return 0;
}