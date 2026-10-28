// Behaviour test for CEngineClothSimulationMeshBase's scalar deleting destructor (0x00B84120): ClearParticleSims, the member arrays destroyed, the base destroyed, memory freed only on flag bit 0.
#include <new>
#include <cstdio>
static int g_log[8]; static int g_n;
struct Base { virtual ~Base() { g_log[g_n++] = 3; } };
struct Arr { ~Arr() { g_log[g_n++] = 2; } };
struct Mesh : Base { Arr ParticleSims; void ClearParticleSims() { g_log[g_n++] = 1; } ~Mesh() { ClearParticleSims(); }
    static int frees; void* ScalarDelete(unsigned int flags) { this->~Mesh(); if (flags & 1) ++frees; return this; } };
int Mesh::frees = 0;
int main()
{
    unsigned char storage[sizeof(Mesh)]; Mesh* m = new (storage) Mesh;
    g_n = 0; m->ScalarDelete(1);
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || Mesh::frees != 1) { std::printf("00b84120_TEST FAIL\n"); return 1; }
    m = new (storage) Mesh; g_n = 0; m->ScalarDelete(0);
    if (g_n != 3 || Mesh::frees != 1) { std::printf("00b84120_TEST FAIL\n"); return 1; }
    std::printf("00b84120_TEST PASS\n");
    return 0;
}