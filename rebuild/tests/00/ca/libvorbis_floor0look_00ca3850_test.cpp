#include <cstdio>
#include <cstdlib>
// floor0_look: calloc a look; m = order, ln = barkmap, vi = info, linearmap = calloc(2, ptr).
struct vorbis_info_floor0 { int order; int rate; int barkmap; int ampbits; int ampdB; int numbooks; int books[16]; float lessthan; float greaterthan; };
struct vorbis_look_floor0 { int ln; int m; int** linearmap; int n[2]; vorbis_info_floor0* vi; int bits; int frames; };
static void* floor0_look(void* vd, void* i)
{
    vorbis_info_floor0* info = (vorbis_info_floor0*)i;
    vorbis_look_floor0* look = (vorbis_look_floor0*)std::calloc(1, sizeof(*look));
    look->m = info->order;
    look->ln = info->barkmap;
    look->vi = info;
    look->linearmap = (int**)std::calloc(2, sizeof(*look->linearmap));
    return look;
}
int main()
{
    vorbis_info_floor0 info; info.order = 30; info.rate = 44100; info.barkmap = 256;
    vorbis_look_floor0* look = (vorbis_look_floor0*)floor0_look(0, &info);
    if (look->m != 30 || look->ln != 256 || look->vi != &info || look->n[0] != 0 || look->bits != 0) { std::printf("00ca3850_TEST FAIL\n"); return 1; }
    if (!look->linearmap || look->linearmap[0] != 0 || look->linearmap[1] != 0) { std::printf("00ca3850_TEST FAIL\n"); return 1; }
    std::free(look->linearmap); std::free(look);
    std::printf("00ca3850_TEST PASS\n");
    return 0;
}