#include <cstdio>

float g_MaxDrawDistance = 1234.5f;

namespace NParticleEngine { float __fastcall GetMaxSpawnDistance(); }

int main()
{
    float v = NParticleEngine::GetMaxSpawnDistance();
    if (v == 1234.5f)
        printf("00aeaa60_TEST PASS\n");
    else
        printf("FAIL %f\n", v);
    return 0;
}