// Test for 0x00C4E350 CEAXManager::GetEffectProperty: the four arguments and an out word reach the effect, its result returns.
#include <cstdio>
struct CEffect { unsigned int id, size, flags; void* data; bool gotOut;
    long GetProperty(unsigned int i, void* d, unsigned int s, unsigned int f, unsigned int* out) { id = i; data = d; size = s; flags = f; gotOut = out != 0; *out = 4; return -3; } };
struct CEAXManager { CEffect* m_pEffect;
    long GetEffectProperty(unsigned int id, void* data, unsigned int size, unsigned int flags) { unsigned int returned; return m_pEffect->GetProperty(id, data, size, flags, &returned); } };
int main()
{
    int dummy; CEffect e; CEAXManager m; m.m_pEffect = &e;
    long r = m.GetEffectProperty(6, &dummy, 4, 9);
    if (r != -3 || e.id != 6 || e.data != &dummy || e.size != 4 || e.flags != 9 || !e.gotOut) { std::printf("00c4e350_TEST FAIL\n"); return 1; }
    std::printf("00c4e350_TEST PASS\n");
    return 0;
}