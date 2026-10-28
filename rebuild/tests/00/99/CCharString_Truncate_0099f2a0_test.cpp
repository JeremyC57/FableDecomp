// Test: CCharString::Truncate -- longer strings are cut to `length` (to zero: become the empty string), shorter ones are untouched
#include <cstdio>
#include <cstring>
class CCharString { public:
    char data[32]; bool isNull; int copies;
    unsigned long GetLength() const { return isNull ? 0 : (unsigned long)std::strlen(data); }
    void Truncate(unsigned long length)
    {
        if (GetLength() > length) {
            if (length == 0) { data[0] = 0; return; }   // *this = ""
            ++copies;                                   // MakeOwnCopyOfData()
            data[length] = 0;
        }
    }
};
int main()
{
    CCharString a; std::strcpy(a.data, "hello"); a.isNull = false; a.copies = 0;
    a.Truncate(3);
    CCharString b; std::strcpy(b.data, "hi"); b.isNull = false; b.copies = 0;
    b.Truncate(5);
    CCharString c; std::strcpy(c.data, "xyz"); c.isNull = false; c.copies = 0;
    c.Truncate(0);
    if (std::strcmp(a.data, "hel") || a.copies != 1 || std::strcmp(b.data, "hi") || b.copies || c.data[0] || c.copies) { std::printf("0099f2a0_TEST FAIL\n"); return 1; }
    std::printf("0099f2a0_TEST PASS\n");
    return 0;
}