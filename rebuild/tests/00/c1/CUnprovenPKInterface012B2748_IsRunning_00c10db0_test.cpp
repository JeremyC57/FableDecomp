// behaviour test for 00c10db0 (standalone copy of the model)
#include <cstdio>
#include <cstring>

class CWorld { public: bool Paused; };
struct I { CWorld* m_pWorld; virtual ~I() {} virtual int IsRunning() { return !m_pWorld->Paused; } };

int main()
{
    CWorld w; w.Paused = false; I i; i.m_pWorld = &w;
    if (!(i.IsRunning() == 1)) { std::printf("00c10db0_TEST FAIL\n"); return 1; }
    w.Paused = true;
    if (!(i.IsRunning() == 0)) { std::printf("00c10db0_TEST FAIL\n"); return 1; }
    std::printf("00c10db0_TEST PASS\n");
    return 0;
}