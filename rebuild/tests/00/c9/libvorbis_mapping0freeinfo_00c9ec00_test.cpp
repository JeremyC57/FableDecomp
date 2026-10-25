// Behaviour test for 00c9ec00: libvorbis mapping0_free_info -- a block is wiped (all 0xC88 bytes) before it is freed; NULL is ignored
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef struct {                                   // libvorbis 1.0.1 mapping0.c vorbis_info_mapping0, 0xC88 bytes
    int submaps;
    int chmuxlist[256];
    int floorsubmap[16];
    int residuesubmap[16];
    int coupling_steps;
    int coupling_mag[256];
    int coupling_ang[256];
} vorbis_info_mapping0;
static int g_freed; static void* g_last; static int g_zero;
static void my_free(void* p) { ++g_freed; g_last = p; const unsigned char* b = (const unsigned char*)p; g_zero = 1; for (unsigned int k = 0; k < sizeof(vorbis_info_mapping0); ++k) if (b[k]) g_zero = 0; std::free(p); }
static void mapping0_free_info(vorbis_info_mapping0* info)
{
    if (info) {
        std::memset(info, 0, sizeof(*info));
        my_free(info);
    }
}
int main()
{
    int ok = sizeof(vorbis_info_mapping0) == 0xC88;
    mapping0_free_info(0); ok &= g_freed == 0;
    vorbis_info_mapping0* p = (vorbis_info_mapping0*)std::malloc(sizeof *p); std::memset(p, 0x5A, sizeof *p);
    mapping0_free_info(p);
    ok &= g_freed == 1 && g_last == p && g_zero == 1;
    std::printf(ok ? "00c9ec00_TEST PASS\n" : "00c9ec00_TEST FAIL\n");
    return ok ? 0 : 1;
}
