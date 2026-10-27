// behaviour test for 00b80930 (standalone copy of the model)
#include <cstdio>

struct P { int x; };
struct CArr { P* _M_start; P* _M_finish; P* _M_end_of_storage; bool empty() const { return _M_start == _M_finish; } };
struct CEngineClothSimulationMeshBase { CArr ParticleSims; bool IsRunning() const { return ParticleSims.empty() ? false : true; } };

int main()
{
    P buf[2]; CEngineClothSimulationMeshBase m; m.ParticleSims._M_start = buf; m.ParticleSims._M_finish = buf; m.ParticleSims._M_end_of_storage = buf + 2;
    if (!(!m.IsRunning())) { std::printf("00b80930_TEST FAIL\n"); return 1; }
    m.ParticleSims._M_finish = buf + 1;
    if (!(m.IsRunning())) { std::printf("00b80930_TEST FAIL\n"); return 1; }
    std::printf("00b80930_TEST PASS\n");
    return 0;
}