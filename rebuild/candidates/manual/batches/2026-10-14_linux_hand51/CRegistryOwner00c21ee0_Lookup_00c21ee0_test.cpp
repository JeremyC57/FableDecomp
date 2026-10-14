#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_key = 0; struct CRegistry { virtual long Find(long* key) { g_key = *key; return *key + 1; } };
struct COwner { CRegistry* m_pRegistry; long Lookup(long key) { long value = key; return m_pRegistry->Find(&value); } };

int main() {
    COwner o; CRegistry r; o.m_pRegistry = &r; if (o.Lookup(42) != 43 || g_key != 42) { std::printf("00c21ee0_TEST FAIL\n"); return 1; }
    std::printf("00c21ee0_TEST PASS\n"); return 0;
}
