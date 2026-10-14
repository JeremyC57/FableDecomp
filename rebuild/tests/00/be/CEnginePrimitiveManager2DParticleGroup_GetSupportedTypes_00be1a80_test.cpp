#include <cstdio>
#include <new>
static int g_grow = 0; static long g_growValue = 0;
struct SScalarTag { SScalarTag() { } };
struct CRetailVector_dword { long* first; long* last; long* end; void InsertGrow(long*, const long& v, const SScalarTag&, unsigned long, bool) { ++g_grow; g_growValue = v; }
    void push_back(const long& value) { if (last != end) { new (last) long(value); ++last; } else InsertGrow(last, value, SScalarTag(), 1, true); } };
struct CEnginePrimitiveManager2DParticleGroup { void GetSupportedTypes(CRetailVector_dword& types) { long value = 0x20; types.push_back(value); } };
int main() {
    long buf[2]; CRetailVector_dword v = { buf, buf, buf + 2 }; CEnginePrimitiveManager2DParticleGroup m; m.GetSupportedTypes(v);
    if (v.last != buf + 1 || buf[0] != 0x20 || g_grow != 0) { std::printf("00be1a80_TEST FAIL\n"); return 1; }
    m.GetSupportedTypes(v); m.GetSupportedTypes(v); if (g_grow != 1 || g_growValue != 0x20 || buf[1] != 0x20) { std::printf("00be1a80_TEST FAIL\n"); return 1; }
    std::printf("00be1a80_TEST PASS\n"); return 0;
}