#include <cstdio>

#include <new>
#include <cstdlib>
#include <cstring>
struct SElement28 { unsigned char _body[0x1c]; };
static unsigned int g_mallocSize = 0;
static void* FakeMalloc(unsigned int size) { g_mallocSize = size; return std::malloc(size); }
SElement28* __stdcall AllocateAndCopy_SElement28(unsigned long count, SElement28* first, SElement28* last)
{
    SElement28* buffer = count ? (SElement28*)FakeMalloc(count * sizeof(SElement28)) : 0;
    for (SElement28* p = first; p != last; ++p)
        new ((SElement28*)((char*)buffer + ((char*)p - (char*)first))) SElement28(*p);
    return buffer;
}
int main()
{
    SElement28 src[3]; for (int i = 0; i < 3; ++i) std::memset(src[i]._body, 0x10 + i, 0x1c);
    SElement28* b = AllocateAndCopy_SElement28(5, src, src + 3);
    if (!b || g_mallocSize != 5 * 0x1c || std::memcmp(b, src, 3 * 0x1c) != 0) { std::printf("00454586_TEST FAIL\n"); return 1; }
    std::free(b);
    if (AllocateAndCopy_SElement28(0, src, src) != 0) { std::printf("00454586_TEST FAIL\n"); return 1; }
    std::printf("00454586_TEST PASS\n"); return 0;
}