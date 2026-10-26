// Behaviour test for 0x009E6530 CCharString::FastCompare: identity, null-first, length, then word-wise order.
#include <cstdio>
#include <cstring>
class CCharStringData { public: char* PData; unsigned long StringLength; };
class CCharString { public:
    CCharStringData* PStringData;
    bool FastCompare(const CCharString& other) const
    {
        const CCharStringData* b = other.PStringData;
        const CCharStringData* a = PStringData;
        if (a == b) return false;
        if (a == 0) return true;
        if (b == 0) return false;
        unsigned long length = a->StringLength;
        if (length < b->StringLength) return true;
        if (length > b->StringLength) return false;
        for (unsigned long i = 0; i < length; i += 4) {
            unsigned int wordB, wordA;
            std::memcpy(&wordB, b->PData + i, 4);
            std::memcpy(&wordA, a->PData + i, 4);
            if (wordA < wordB) return true;
            if (wordA > wordB) return false;
        }
        return false;
    }
};
static char bufA[16] = "abcd", bufB[16] = "abce", bufC[16] = "ab";
int main()
{
    CCharStringData da = { bufA, 4 }, db = { bufB, 4 }, dc = { bufC, 2 };
    CCharString a = { &da }, b = { &db }, c = { &dc }, n = { 0 };
    if (a.FastCompare(a) || !n.FastCompare(a) || a.FastCompare(n) || n.FastCompare(n)) { std::printf("009e6530_TEST FAIL\n"); return 1; }
    if (!c.FastCompare(a) || a.FastCompare(c)) { std::printf("009e6530_TEST FAIL\n"); return 1; }    // shorter first
    if (a.FastCompare(b) == b.FastCompare(a)) { std::printf("009e6530_TEST FAIL\n"); return 1; }      // strict
    CCharStringData da2 = { bufA, 4 }; CCharString a2 = { &da2 };
    if (a.FastCompare(a2) || a2.FastCompare(a)) { std::printf("009e6530_TEST FAIL\n"); return 1; }    // equal text
    std::printf("009e6530_TEST PASS\n");
    return 0;
}