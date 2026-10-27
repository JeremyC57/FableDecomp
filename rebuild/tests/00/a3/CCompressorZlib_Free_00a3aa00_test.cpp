// Test for 0x00A3AA00 CCompressorZlib::Free: zlib free hook -- frees the address, ignoring the opaque pointer.
#include <stdio.h>
#include <stdlib.h>
static void* g_freed = 0;
static void FreeMock(void* p) { g_freed = p; free(p); }
class CCompressorZlib { public: static void Free(void* opaque, void* address) { FreeMock(address); } };
int main()
{
    void* p = malloc(8); int opaque;
    CCompressorZlib::Free(&opaque, p);
    bool ok = g_freed == p;
    puts(ok ? "00a3aa00_TEST PASS" : "00a3aa00_TEST FAIL");
    return ok ? 0 : 1;
}