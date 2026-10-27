// Behaviour test for 0x00CA3600 floor0_unpack (standalone copy of the logic; oggpack_read fed from a table)
#include <cstdio>
#include <cstdlib>
struct vorbis_info_floor0 { int order; int rate; int barkmap; int ampbits; int ampdB; int numbooks; int books[16]; float lessthan, greaterthan; };
static const int* g_vals; static int g_pos; static int g_freed;
static int oggpack_read(int bits) { (void)bits; return g_vals[g_pos++]; }
static void floor0_free_info(vorbis_info_floor0* i) { if (i) { ++g_freed; std::free(i); } }
static vorbis_info_floor0* floor0_unpack(int books)
{
    int j;
    vorbis_info_floor0* info = (vorbis_info_floor0*)std::malloc(sizeof(*info));
    info->order = oggpack_read(8); info->rate = oggpack_read(16); info->barkmap = oggpack_read(16);
    info->ampbits = oggpack_read(6); info->ampdB = oggpack_read(8); info->numbooks = oggpack_read(4) + 1;
    if (info->order < 1) goto err_out;
    if (info->rate < 1) goto err_out;
    if (info->barkmap < 1) goto err_out;
    if (info->numbooks < 1) goto err_out;
    for (j = 0; j < info->numbooks; j++) {
        info->books[j] = oggpack_read(8);
        if (info->books[j] < 0 || info->books[j] >= books) goto err_out;
    }
    return info;
err_out:
    floor0_free_info(info);
    return 0;
}
int main()
{
    static const int ok[] = { 22, 44100, 64, 6, 120, 1, 3, 4 };
    g_vals = ok; g_pos = 0;
    vorbis_info_floor0* f = floor0_unpack(5);
    if (!f || f->numbooks != 2 || f->books[1] != 4 || g_pos != 8) { std::printf("00ca3600_TEST FAIL\n"); return 1; }
    std::free(f);
    static const int bad[] = { 22, 44100, 64, 6, 120, 1, 3, 5 };
    g_vals = bad; g_pos = 0;
    if (floor0_unpack(5) != 0 || g_freed != 1) { std::printf("00ca3600_TEST FAIL\n"); return 1; }
    static const int zero[] = { 0, 44100, 64, 6, 120, 0 };
    g_vals = zero; g_pos = 0;
    if (floor0_unpack(5) != 0 || g_freed != 2 || g_pos != 6) { std::printf("00ca3600_TEST FAIL\n"); return 1; }
    std::printf("00ca3600_TEST PASS\n");
    return 0;
}