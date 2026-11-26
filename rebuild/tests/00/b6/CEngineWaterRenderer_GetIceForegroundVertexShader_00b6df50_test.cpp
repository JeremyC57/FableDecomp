// Behaviour test for 0x00B6DF50 CEngineWaterRenderer::GetIceForegroundVertexShader: the row's defines, includes and body verbatim, run on a zeroed
// object image whose CArray<CVertexShader> IceForegroundVertexShader (+0x12c: _M_start, _M_finish) spans four 8-byte shaders. In-range counts
// return their own shader, the last index returns the last one, larger counts and negative counts (unsigned compare) clamp to the last.
#include <stdio.h>
#include <string.h>
#define private public   // test access to the private helper only
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINEWATERRENDERER
#define FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS
#include "engine/CEngineWaterRenderer.h"

CVertexShader& CEngineWaterRenderer::GetIceForegroundVertexShader(long point_light_count)
{
    return IceForegroundVertexShader[(unsigned long)point_light_count < IceForegroundVertexShader.size() - 1 ? point_light_count : IceForegroundVertexShader.size() - 1];
}

static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static double image[0x800 / 8];
static double shaders[4];
int main()
{
    memset(image, 0, sizeof(image));
    char* base = (char*)image;
    char* first = (char*)shaders;
    *(char**)(base + 0x12c) = first;
    *(char**)(base + 0x12c + 4) = first + 4 * 8;
    *(char**)(base + 0x12c + 8) = first + 4 * 8;
    CEngineWaterRenderer* r = (CEngineWaterRenderer*)image;
    check((char*)&r->GetIceForegroundVertexShader(0) == first, "count 0");
    check((char*)&r->GetIceForegroundVertexShader(2) == first + 16, "count 2");
    check((char*)&r->GetIceForegroundVertexShader(3) == first + 24, "count 3 (last)");
    check((char*)&r->GetIceForegroundVertexShader(4) == first + 24, "count 4 clamps");
    check((char*)&r->GetIceForegroundVertexShader(9) == first + 24, "count 9 clamps");
    check((char*)&r->GetIceForegroundVertexShader(-1) == first + 24, "negative count clamps (unsigned)");
    *(char**)(base + 0x12c + 4) = first + 2 * 8;
    check((char*)&r->GetIceForegroundVertexShader(1) == first + 8 && (char*)&r->GetIceForegroundVertexShader(2) == first + 8, "size 2");
    if (!g_fail) printf("00b6df50_TEST PASS\n");
    return g_fail;
}