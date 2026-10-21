// test for 0066ebe0 DeregisterAllScreens
#include <stdio.h>
class CWorld;
template <class T> class vector {                        // STLport vector<T>
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
namespace NHeroInformationScreens {
class CBase;
class CManager {
public:
    void DeregisterAllScreens();
    void InitialiseScrollBarIndices(long first);         // 0x0066EA85
    CWorld*              World;                          // +0x00
    vector<const CBase*> Screens;                        // +0x04
};
void CManager::DeregisterAllScreens()
{
    for (unsigned int i = 0; i < Screens.size(); ++i)
        Screens[i] = 0;
    InitialiseScrollBarIndices(0);
}
}

static long g_init = -1; static int g_calls;
void NHeroInformationScreens::CManager::InitialiseScrollBarIndices(long first) { g_init = first; ++g_calls; }
int main()
{
    using namespace NHeroInformationScreens;
    static const CBase* screens[3] = { (const CBase*)1, (const CBase*)2, (const CBase*)3 };
    CManager m; m.World = 0;
    m.Screens._M_start = screens; m.Screens._M_finish = screens + 3; m.Screens._M_end_of_storage = screens + 3;
    m.DeregisterAllScreens();
    int ok = screens[0] == 0 && screens[1] == 0 && screens[2] == 0 && g_init == 0 && g_calls == 1;
    m.Screens._M_finish = screens; m.DeregisterAllScreens(); ok &= g_calls == 2;
    printf(ok ? "0066ebe0_TEST PASS\n" : "0066ebe0_TEST FAIL\n");
    return ok ? 0 : 1;
}