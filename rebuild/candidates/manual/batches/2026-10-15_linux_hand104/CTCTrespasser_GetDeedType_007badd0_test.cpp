#include <cstdio>
struct CTCTrespasser {
    long GetDeedType(long trespassType)
    {
        switch (trespassType) {
        case 0: return 10;
        case 1: return 11;
        case 2: return 2;
        }
        return 0;
    }
};
int main()
{
    CTCTrespasser t;
    if (t.GetDeedType(0) != 10 || t.GetDeedType(1) != 11 || t.GetDeedType(2) != 2 || t.GetDeedType(3) != 0 || t.GetDeedType(-1) != 0) {
        std::printf("007badd0_TEST FAIL\n"); return 1;
    }
    std::printf("007badd0_TEST PASS\n");
    return 0;
}
