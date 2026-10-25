// Behaviour test for 0x00CA35A0 floor0_free_look (libvorbis floor0.c): free both linear maps and the map table, zero the
// look and free it; a NULL look is ignored. The CRT free is replaced by a recording stand-in.
#include <cstdio>
#include <cstring>
static void* g_freed[8];
static int g_nfreed;
static int g_zeroed_before_free;
struct vorbis_look_floor0 {
    int ln;
    int m;
    int** linearmap;
    int n[2];
    void* vi;
    long bits;
    long frames;
};
static vorbis_look_floor0* g_look;
static void my_free(void* p)
{
    if (p == g_look) g_zeroed_before_free = (g_look->ln == 0 && g_look->m == 0 && g_look->linearmap == 0 && g_look->bits == 0);
    g_freed[g_nfreed++] = p;
}
#define _ogg_free my_free
void floor0_free_look(void* i)
{
    vorbis_look_floor0* look = (vorbis_look_floor0*)i;
    if (look) {

        if (look->linearmap) {

            if (look->linearmap[0]) _ogg_free(look->linearmap[0]);
            if (look->linearmap[1]) _ogg_free(look->linearmap[1]);

            _ogg_free(look->linearmap);
        }
        std::memset(look, 0, sizeof(*look));
        _ogg_free(look);
    }
}
int main()
{
    int map0[4], map1[4];
    int* maps[2] = { map0, map1 };
    vorbis_look_floor0 look;
    look.ln = 64; look.m = 16; look.linearmap = maps; look.n[0] = 128; look.n[1] = 1024; look.vi = 0; look.bits = 7; look.frames = 3;
    g_look = &look;
    int ok = 1;
    floor0_free_look(0);
    ok &= g_nfreed == 0;
    floor0_free_look(&look);
    ok &= g_nfreed == 4 && g_freed[0] == map0 && g_freed[1] == map1 && g_freed[2] == maps && g_freed[3] == &look;
    ok &= g_zeroed_before_free;
    g_nfreed = 0; maps[1] = 0;
    look.ln = 1; look.linearmap = maps;
    floor0_free_look(&look);
    ok &= g_nfreed == 3 && g_freed[0] == map0 && g_freed[1] == maps && g_freed[2] == &look;
    g_nfreed = 0; look.linearmap = 0;
    floor0_free_look(&look);
    ok &= g_nfreed == 1 && g_freed[0] == &look;
    if (!ok) { std::printf("00ca35a0_TEST FAIL\n"); return 1; }
    std::printf("00ca35a0_TEST PASS\n");
    return 0;
}