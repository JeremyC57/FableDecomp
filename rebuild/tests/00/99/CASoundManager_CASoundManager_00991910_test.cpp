// Behaviour test for 0x00991910 CASoundManager::CASoundManager(init, flag): settings copied to their slots, the scale's
// reciprocal stored, volume 1, flag kept, the three enables true, vector empty, busy false; a new resource bank is
// installed, its resource list gets the init memory budget and a zero unload delay.
#include <cstdio>
#include <cmath>
class CResourceList { public: long MaximumMemory; unsigned long UnloadDelay; int makeFreeCalls;
    void SetMaximumMemory(unsigned long m) { MaximumMemory = (long)m; ++makeFreeCalls; } };
class CResourceBank { public: CResourceList ResourceList; CResourceBank() { ResourceList.MaximumMemory = -1; ResourceList.UnloadDelay = 77; ResourceList.makeFreeCalls = 0; } };
template <class T> class CCountedPointer { public:
    T* Data; int resets;
    CCountedPointer() : Data(0), resets(0) { }
    void Reset(T* item) { delete Data; Data = item; ++resets; }
    T* operator->() const { return Data; }
};
struct CSoundManagerInit { int s00, s04, s08, s0c; float scale; int s14, s18; unsigned int memory; };
struct CASoundManager {
    int s08, s0c; float scale, inverse; int s04, s00; float volume; bool flag; bool e24, e25, e26;
    CCountedPointer<CResourceBank> bank; void* vstart; void* vfinish; void* veos; bool busy;
    CASoundManager(const CSoundManagerInit& init, bool f)
        : s08(init.s08), s0c(init.s0c), scale(init.scale), inverse(1.0f / init.scale), s04(init.s04), s00(init.s00),
          volume(1.0f), flag(f), e24(true), e25(true), e26(true), vstart(0), vfinish(0), veos(0), busy(false)
    {
        bank.Reset(new CResourceBank);
        bank->ResourceList.SetMaximumMemory(init.memory);
        bank->ResourceList.UnloadDelay = 0;
    }
};
int main()
{
    int fails = 0;
    CSoundManagerInit init = { 1, 2, 3, 4, 4.0f, 0, 0, 0x200000 };
    CASoundManager m(init, true);
    if (m.s00 != 1 || m.s04 != 2 || m.s08 != 3 || m.s0c != 4 || m.scale != 4.0f || std::fabs(m.inverse - 0.25f) > 1e-6f) ++fails;
    if (m.volume != 1.0f || !m.flag || !m.e24 || !m.e25 || !m.e26 || m.vstart || m.vfinish || m.veos || m.busy) ++fails;
    if (!m.bank.Data || m.bank.resets != 1 || m.bank->ResourceList.MaximumMemory != 0x200000 || m.bank->ResourceList.UnloadDelay != 0
        || m.bank->ResourceList.makeFreeCalls != 1) ++fails;
    CASoundManager n(init, false);
    if (n.flag || n.bank.Data == m.bank.Data) ++fails;
    delete m.bank.Data; delete n.bank.Data;
    std::printf(fails ? "00991910_TEST FAIL\n" : "00991910_TEST PASS\n");
    return fails ? 1 : 0;
}