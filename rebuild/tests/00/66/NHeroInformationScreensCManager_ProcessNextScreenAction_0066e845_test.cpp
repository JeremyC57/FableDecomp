#include <cstdio>
class CWorld;
template <class T> class vector {                        // STLport vector<T>
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
namespace NHeroInformationScreens {
class CBase {
public:
    virtual void _v00() const;
    virtual void _v01() const;
    virtual void _v02() const;
    virtual bool IsDisplayable() const;                  // slot 3 (+0x0c, name not confirmed)
};
class CManager {
public:
    CWorld*              World;                          // +0x00
    vector<const CBase*> Screens;                        // +0x04
    vector<const CBase*> InventoryScreens;               // +0x10
    vector<long>         ScrollBarIndices;               // +0x1c
    long                 CurrentScreen;                  // +0x28
    long                 LastScreen;                     // +0x2c
    long                 HighlightedScreen;              // +0x30
    const CBase* GetScreenAtScrollIndex(long scroll_index);   // 0x0066E7E1
    void ProcessNextScreenAction(long no_slots, long no_screens);
};
void CManager::ProcessNextScreenAction(long no_slots, long no_screens)
{
    ++HighlightedScreen;
    if (HighlightedScreen > no_slots - 1)
    {
        HighlightedScreen = no_slots - 1;
        for (long i = 0; i < no_slots; ++i)
        {
            do
            {
                ++ScrollBarIndices[i];
                if (ScrollBarIndices[i] > no_screens - 1)
                    ScrollBarIndices[i] -= no_screens;
            } while (!GetScreenAtScrollIndex(i)->IsDisplayable());
        }
    }
}
}

namespace NHeroInformationScreens {
static bool g_disp[8];
class CTestScreen : public CBase {
public:
    long Id;
    void _v00() const {} void _v01() const {} void _v02() const {}
    bool IsDisplayable() const { return g_disp[Id]; }
};
void CBase::_v00() const {} void CBase::_v01() const {} void CBase::_v02() const {}
bool CBase::IsDisplayable() const { return false; }
static CTestScreen g_screens[8];
const CBase* CManager::GetScreenAtScrollIndex(long scroll_index) { return &g_screens[ScrollBarIndices[scroll_index]]; }
}
using namespace NHeroInformationScreens;
int main()
{
    for (int k = 0; k < 8; ++k) { g_screens[k].Id = k; g_disp[k] = true; }
    static long idx[3];
    CManager m; m.ScrollBarIndices._M_start = idx; m.ScrollBarIndices._M_finish = idx + 3;
    idx[0] = 0; idx[1] = 1; idx[2] = 2;
    m.HighlightedScreen = 0;
    m.ProcessNextScreenAction(3, 5);
    bool ok = m.HighlightedScreen == 1 && idx[0] == 0 && idx[2] == 2;
    m.HighlightedScreen = 2;
    g_disp[3] = false;                       // slot 1: 1 -> 2; slot 0: 0 -> 1; slot 2: 2 -> 3 (skip) -> 4
    m.ProcessNextScreenAction(3, 5);
    ok = ok && m.HighlightedScreen == 2 && idx[0] == 1 && idx[1] == 2 && idx[2] == 4;
    m.ProcessNextScreenAction(3, 5);         // 1->2, 2->3 (skip) ->4, 4->0 (wrap)
    ok = ok && idx[0] == 2 && idx[1] == 4 && idx[2] == 0;
    std::printf(ok ? "0066e845_TEST PASS\n" : "0066e845_TEST FAIL\n");
    return ok ? 0 : 1;
}