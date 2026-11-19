// Behaviour test for 0x00A0C1B0 CCamera::MoveSideways: the row's defines, includes and body verbatim, on a camera image (members private:
// WorldPosition +0x00, RHSet Up +0x0C / Forward +0x18, HeightLocked +0x24). Right = Forward x Up; free: the position moves by
// right * world_units in all three axes; height-locked: X and Y only (Z kept, even when the right axis has a Z component).
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCAMERA_MOVESIDEWAYS
#include "engine/CCamera.h"

void CCamera::MoveSideways(const float world_units)
{
    if (HeightLocked) {
        WorldPosition.X += RHSet.GetRight().X * world_units;
        WorldPosition.Y += RHSet.GetRight().Y * world_units;
    } else {
        WorldPosition += RHSet.GetRight() * world_units;
    }
}

static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static float g_img[16];
static void Setup(float fx, float fy, float fz, float ux, float uy, float uz, bool locked)
{
    memset(g_img, 0, sizeof(g_img));
    g_img[0] = 1.0f; g_img[1] = 2.0f; g_img[2] = 3.0f;
    g_img[3] = ux; g_img[4] = uy; g_img[5] = uz;
    g_img[6] = fx; g_img[7] = fy; g_img[8] = fz;
    reinterpret_cast<unsigned char*>(g_img)[0x24] = locked ? 1 : 0;
}
int main()
{
    CCamera* cam = reinterpret_cast<CCamera*>(g_img);
    // Forward (0,1,0) x Up (0,0,1) = (1,0,0)
    Setup(0, 1, 0, 0, 0, 1, false);
    cam->MoveSideways(2.0f);
    check(g_img[0] == 3.0f && g_img[1] == 2.0f && g_img[2] == 3.0f, "free: along +X");
    // a general pair: F (1,2,3) x U (4,5,6) = (2*6-3*5, 3*4-1*6, 1*5-2*4) = (-3, 6, -3)
    Setup(1, 2, 3, 4, 5, 6, false);
    cam->MoveSideways(0.5f);
    check(g_img[0] == -0.5f && g_img[1] == 5.0f && g_img[2] == 1.5f, "free: full cross product, all axes");
    Setup(1, 2, 3, 4, 5, 6, true);
    cam->MoveSideways(0.5f);
    check(g_img[0] == -0.5f && g_img[1] == 5.0f && g_img[2] == 3.0f, "height locked: Z kept");
    Setup(1, 2, 3, 4, 5, 6, true);
    cam->MoveSideways(-2.0f);
    check(g_img[0] == 7.0f && g_img[1] == -10.0f && g_img[2] == 3.0f, "negative distance");
    if (!g_fail) printf("00a0c1b0_TEST PASS\n");
    return g_fail;
}