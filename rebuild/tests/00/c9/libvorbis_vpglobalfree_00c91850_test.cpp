// Behaviour test for 0x00C91850 libvorbis _vp_global_free: zero the psy-global look, then free it (null-safe)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(_MSC_VER)
#define __fastcall
#endif
#define P_NOISECURVES 3
typedef struct vorbis_info_psy_global vorbis_info_psy_global;
typedef struct {
    float ampmax;
    int channels;
    vorbis_info_psy_global* gi;
    int coupling_pointlimit[2][P_NOISECURVES];
} vorbis_look_psy_global;
static int gFrees = 0;
static bool gZeroedAtFree = false;
static void test_free(void* p)                    // stand-in for the CRT free import
{
    const vorbis_look_psy_global* look = (const vorbis_look_psy_global*)p;
    gZeroedAtFree = look->ampmax < 0.0001f && look->ampmax > -0.0001f && look->channels == 0 && look->gi == 0 &&
                    look->coupling_pointlimit[0][0] == 0 && look->coupling_pointlimit[1][2] == 0;
    ++gFrees;
    free(p);
}
#define _ogg_free test_free
extern "C" void __fastcall _vp_global_free(vorbis_look_psy_global* look)
{
    if (look) {
        memset(look, 0, sizeof(*look));
        _ogg_free(look);
    }
}
int main()
{
    vorbis_look_psy_global* look = (vorbis_look_psy_global*)malloc(sizeof(vorbis_look_psy_global));
    look->ampmax = 3.5f; look->channels = 2; look->gi = (vorbis_info_psy_global*)look;
    look->coupling_pointlimit[0][0] = 7; look->coupling_pointlimit[1][2] = 9;
    bool ok = true;
    _vp_global_free(look);
    ok = ok && gFrees == 1 && gZeroedAtFree;      // cleared before the free
    _vp_global_free(0);
    ok = ok && gFrees == 1;                       // null look: no free
    if (ok) { printf("00c91850_TEST PASS\n"); return 0; }
    printf("00c91850_TEST FAIL\n");
    return 1;
}