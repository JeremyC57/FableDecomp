// behaviour test for 0x00404a60 NUISystem::CIME::Callback_malloc(unsigned int): forwards the size to malloc and returns its block
#include <cstdio>
#include <cstdlib>
static unsigned int gLastSize = 0;
static void* Malloc(unsigned int size) { gLastSize = size; return std::malloc(size); }
static void* Callback_malloc(unsigned int size) { return Malloc(size); }
int main()
{
    int fails = 0;
    unsigned char* p = (unsigned char*)Callback_malloc(37);
    if (!p || gLastSize != 37) fails |= 1;
    for (int i = 0; i < 37; ++i) p[i] = (unsigned char)i;
    if (p[36] != 36) fails |= 2;
    std::free(p);
    if (fails) { std::printf("00404a60_TEST FAIL %d\n", fails); return 1; }
    std::printf("00404a60_TEST PASS\n");
    return 0;
}