// Behaviour test for 0x0049F3F0 CWorld::OpenHeroInformationScreen: every registered screen mode removed, the new mode
// added, the screens opened, the world paused.
#include <cstdio>
#include <cstdlib>
#include <cstring>
enum EPlayerMode { MODE_A = 3, MODE_B = 7, MODE_NEW = 11 };
static char g_log[96];
static void Log(const char* s) { std::strcat(g_log, s); }
struct CBase { EPlayerMode m; virtual ~CBase() { } virtual EPlayerMode GetPlayerMode() const { return m; } };
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) std::free(_M_start); }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
static CBase g_s1, g_s2;
struct CManager {
    void GetAllRegisteredScreens(CRetailVector<const CBase*>* v)
    { v->_M_start = (const CBase**)std::malloc(2 * sizeof(CBase*)); v->_M_start[0] = &g_s1; v->_M_start[1] = &g_s2; v->_M_finish = v->_M_end_of_storage = v->_M_start + 2; }
    void Open() { Log("open;"); }
};
static CManager g_manager;
struct CPlayerGui { CManager* GetPHeroInfoScreensManager() { return &g_manager; } };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
struct CPlayer {
    void RemoveMode(EPlayerMode m) { char b[16]; std::sprintf(b, "-%d;", (int)m); Log(b); }
    void AddMode(EPlayerMode m, bool f) { char b[16]; std::sprintf(b, "+%d%c;", (int)m, f ? 't' : 'f'); Log(b); }
};
struct CPlayerManager { CPlayer p; CPlayer* GetMainPlayer() { return &p; } };
struct CWorld {
    CPlayerManager* PlayerManager; bool Paused;
    void SetAsPaused(bool paused) { if (Paused != paused) Paused = paused; }
    void OpenHeroInformationScreen(EPlayerMode mode)
    {
        CPlayer* player = PlayerManager->GetMainPlayer();
        CManager* manager = g_pPlayerGui->GetPHeroInfoScreensManager();
        CRetailVector<const CBase*> screens;
        manager->GetAllRegisteredScreens(&screens);
        for (unsigned long i = 0; i < screens.size(); ++i) player->RemoveMode(screens[i]->GetPlayerMode());
        player->AddMode(mode, false);
        g_pPlayerGui->GetPHeroInfoScreensManager()->Open();
        SetAsPaused(true);
    }
};
int main()
{
    g_s1.m = MODE_A; g_s2.m = MODE_B; g_log[0] = 0;
    CPlayerManager pm; CWorld w; w.PlayerManager = &pm; w.Paused = false;
    w.OpenHeroInformationScreen(MODE_NEW);
    bool ok = std::strcmp(g_log, "-3;-7;+11f;open;") == 0 && w.Paused;
    if (!ok) { std::printf("0049f3f0_TEST FAIL\n"); return 1; }
    std::printf("0049f3f0_TEST PASS\n");
    return 0;
}