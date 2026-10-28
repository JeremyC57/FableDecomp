// Test: CMemoryFile ctor -- empty buffer and pathname, then Open(pathname, access, flags) with the constructor's arguments
#include <cstdio>
#include <cstring>
class CMemoryFile { public:
    void* start; void* finish; void* eos; char Pathname[16]; int access; unsigned int flags; int opens;
    CMemoryFile(const char* pathname, int a, unsigned int f) : start(0), finish(0), eos(0), access(0), flags(0), opens(0)
    {
        Pathname[0] = 0;
        Open(pathname, a, f);
    }
    void Open(const char* p, int a, unsigned int f) { std::strcpy(Pathname, p); access = a; flags = f; ++opens; }
};
int main()
{
    CMemoryFile m("x.big", 1, 7);
    if (m.start || m.finish || m.eos || std::strcmp(m.Pathname, "x.big") || m.access != 1 || m.flags != 7 || m.opens != 1) { std::printf("009e97b0_TEST FAIL\n"); return 1; }
    std::printf("009e97b0_TEST PASS\n");
    return 0;
}