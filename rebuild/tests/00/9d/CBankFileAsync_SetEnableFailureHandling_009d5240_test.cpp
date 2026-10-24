// Behaviour test for CBankFileAsync::SetEnableFailureHandling (0x009D5240).
#include <cstdio>

class CBankFileAsync
{
public:
    static void SetEnableFailureHandling(bool enable);
    static unsigned int s_allocationFlags;
};

unsigned int CBankFileAsync::s_allocationFlags = 0x55;

void CBankFileAsync::SetEnableFailureHandling(bool enable)
{
    s_allocationFlags = enable ? 0 : 2;
}

int main()
{
    CBankFileAsync::SetEnableFailureHandling(false);
    if (CBankFileAsync::s_allocationFlags != 2) { std::printf("009d5240_TEST FAIL\n"); return 1; }
    CBankFileAsync::SetEnableFailureHandling(true);
    if (CBankFileAsync::s_allocationFlags != 0) { std::printf("009d5240_TEST FAIL\n"); return 1; }
    std::printf("009d5240_TEST PASS\n");
    return 0;
}