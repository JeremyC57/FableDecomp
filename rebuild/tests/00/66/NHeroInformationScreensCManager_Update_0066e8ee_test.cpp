#include <cstdio>
class CWorld;
namespace NHeroInformationScreens {
class CBase {
public:
    virtual ~CBase();
    virtual void _slot01() const; virtual void _slot02() const;
    virtual bool IsSelectable() const;                   // slot 3 (+0x0c), name not recovered
};
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    T& operator[](unsigned int n) { return _M_start[n]; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    long                  CurrentScreen;                 // +0x28
    long                  LastScreen;                    // +0x2c
    long                  HighlightedScreen;             // +0x30
    const CBase* GetScreenAtScrollIndex(long index);     // 0x0066E7E1
    void ProcessNextScreenAction(long number_of_columns, long number_of_screens);       // 0x0066E845
    void ProcessPreviousScreenAction(long number_of_columns, long number_of_screens);   // 0x0066E8A1
    void Update();
};
void CManager::Update()
{
    const long columns = ScrollBarIndices.size();
    const long screens = InventoryScreens.size();
    if (CurrentScreen != LastScreen)
    {
        if (LastScreen >= screens && CurrentScreen == 0)
            ProcessNextScreenAction(columns, screens);
        else if (LastScreen == 0 && CurrentScreen >= screens)
            ProcessPreviousScreenAction(columns, screens);
        else if (CurrentScreen > LastScreen)
            ProcessNextScreenAction(columns, screens);
        else if (CurrentScreen < LastScreen)
            ProcessPreviousScreenAction(columns, screens);
        LastScreen = CurrentScreen;
    }
}
}


namespace NHeroInformationScreens {
static char g_log[16]; static int g_n; static long g_cols, g_scr;
void CManager::ProcessNextScreenAction(long c, long s) { g_log[g_n++] = 'N'; g_cols = c; g_scr = s; }
void CManager::ProcessPreviousScreenAction(long c, long s) { g_log[g_n++] = 'P'; g_cols = c; g_scr = s; }
}
using namespace NHeroInformationScreens;
int main()
{
    static CManager m; static long idx[3]; static const CBase* inv[5];
    m.ScrollBarIndices._M_start = idx; m.ScrollBarIndices._M_finish = idx + 3;
    m.InventoryScreens._M_start = inv; m.InventoryScreens._M_finish = inv + 5;
    bool ok = true;
    m.CurrentScreen = 2; m.LastScreen = 2; m.Update(); ok = ok && g_n == 0;
    m.CurrentScreen = 3; m.Update(); ok = ok && g_n == 1 && g_log[0] == 'N' && m.LastScreen == 3 && g_cols == 3 && g_scr == 5;
    m.CurrentScreen = 1; m.Update(); ok = ok && g_log[1] == 'P' && m.LastScreen == 1;
    m.LastScreen = 5; m.CurrentScreen = 0; m.Update(); ok = ok && g_log[2] == 'N';
    m.LastScreen = 0; m.CurrentScreen = 6; m.Update(); ok = ok && g_log[3] == 'P' && m.LastScreen == 6;
    if (ok) std::printf("0066e8ee_TEST PASS\n");
    return 0;
}