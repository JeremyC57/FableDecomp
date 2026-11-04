#include <cstdio>
class CMeshDataBank { public: long Value; };
extern "C" const CMeshDataBank* g_GFPreMainMemoryUsed;
const CMeshDataBank* g_GFPreMainMemoryUsed = 0;

namespace NParticleEngine { const CMeshDataBank& __fastcall GetMeshBank(); }

int main()
{
    static CMeshDataBank bank;
    g_GFPreMainMemoryUsed = &bank;
    const CMeshDataBank& v = NParticleEngine::GetMeshBank();
    if (&v == &bank)
        printf("00aeaab0_TEST PASS\n");
    else
        printf("FAIL %p\n", (const void*)&v);
    return 0;
}
