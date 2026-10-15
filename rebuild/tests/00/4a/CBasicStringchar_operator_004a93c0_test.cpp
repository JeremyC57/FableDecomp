// Behaviour test for CBasicString<char>::operator== @ 0x004A93C0: length gate, then the ordered compare must be 0.
#include <cstdio>
struct CBasicString {
    const char*   PData;
    unsigned long StringLength;
    static long Compare(const char* a, const char* b)
    {
        for (;;) {
            if (*a == 0 && *b == 0) return 0;
            if (*a < *b) return -1;
            if (*a > *b) return 1;
            ++a; ++b;
        }
    }
    bool operator==(const CBasicString& other) const
    {
        if (StringLength != other.StringLength) return false;
        return Compare(PData, other.PData) == 0;
    }
};
int main()
{
    CBasicString a = { "Hero", 4 }, b = { "Hero", 4 }, c = { "Herp", 4 }, d = { "Hero", 5 };
    if (!(a == b) || (a == c) || (a == d)) { std::printf("004a93c0_TEST FAIL\n"); return 1; }
    std::printf("004a93c0_TEST PASS\n");
    return 0;
}