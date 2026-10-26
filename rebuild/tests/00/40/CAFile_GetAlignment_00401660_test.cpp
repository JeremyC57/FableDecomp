#include <cstdio>

class CAFile;
static long CAFile_GetAlignment(const CAFile* file)
{
    (void)file;
    return 1;
}

int main()
{
    long r = CAFile_GetAlignment((const CAFile*)0);
    if (r != 1) {
        std::printf("CAFile_00401660_TEST FAIL: got %ld\n", r);
        return 1;
    }
    std::printf("CAFile_00401660_TEST PASS\n");
    return 0;
}