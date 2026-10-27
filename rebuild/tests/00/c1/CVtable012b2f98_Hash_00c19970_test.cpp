// behaviour test for 00c19970 (standalone copy of the model)
#include <cstdio>
#include <cstring>

static unsigned long JenkinsStub(const char* data, unsigned long length, unsigned long seed) { unsigned long h = seed; for (unsigned long i = 0; i < length; i++) h = h * 31 + (unsigned char)data[i]; return h + length * 1000; }
struct H { virtual ~H() {} virtual unsigned long Hash(const char* key) { if (!key) return 0xFFFFFFFFUL; return JenkinsStub(key, (unsigned long)std::strlen(key), 0); } };

int main()
{
    H h;
    if (!(h.Hash(0) == 0xFFFFFFFFUL && h.Hash("") == 0 && h.Hash("ab") == (97UL * 31 + 98) + 2000)) { std::printf("00c19970_TEST FAIL\n"); return 1; }
    std::printf("00c19970_TEST PASS\n");
    return 0;
}