// Behaviour test for 00c9fdd0: libvorbis res0_free_info -- a block is wiped (all 0x714 bytes) before it is freed; NULL is ignored
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef struct vorbis_info_residue0 {              // libvorbis 1.0.1 backends.h, 0x714 bytes
    long  begin;
    long  end;
    int   grouping;
    int   partitions;
    int   groupbook;
    int   secondstages[64];
    int   booklist[256];
    float classmetric1[64];
    float classmetric2[64];
} vorbis_info_residue0;
static int g_freed; static void* g_last; static int g_zero;
static void my_free(void* p) { ++g_freed; g_last = p; const unsigned char* b = (const unsigned char*)p; g_zero = 1; for (unsigned int k = 0; k < sizeof(vorbis_info_residue0); ++k) if (b[k]) g_zero = 0; std::free(p); }
static void res0_free_info(vorbis_info_residue0* info)
{
    if (info) {
        std::memset(info, 0, sizeof(*info));
        my_free(info);
    }
}
int main()
{
    int ok = sizeof(vorbis_info_residue0) == 0x714;
    res0_free_info(0); ok &= g_freed == 0;
    vorbis_info_residue0* p = (vorbis_info_residue0*)std::malloc(sizeof *p); std::memset(p, 0x5A, sizeof *p);
    res0_free_info(p);
    ok &= g_freed == 1 && g_last == p && g_zero == 1;
    std::printf(ok ? "00c9fdd0_TEST PASS\n" : "00c9fdd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
