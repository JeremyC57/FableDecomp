// Behaviour test for 0x0066E985 NHeroInformationScreens::CManager::RemoveAllPlayerModes: every registered (non-null)
// screen's player mode is removed from the main player, in order; the local vector's buffer is freed.
#include <stdio.h>
#include <stdlib.h>
class CWorld;
enum EPlayerMode { PLAYER_MODE_NONE, PLAYER_MODE_A = 3, PLAYER_MODE_B = 7, PLAYER_MODE_C = 9 };
static int g_removed[16]; static int g_removedCount = 0; static const void* g_player = 0;
class CPlayer { public: int id; void RemoveMode(EPlayerMode mode) { if (this == g_player) g_removed[g_removedCount++] = mode; } };
static CPlayer g_mainPlayer;
class CPlayerManager { public: CPlayer* GetMainPlayer() { return &g_mainPlayer; } };
class CMainGameComponent { public: unsigned char _pad00[0x1c]; CPlayerManager* PPlayerManager; };
static CPlayerManager g_pm; static CMainGameComponent g_mgc; static CMainGameComponent* g_pMainGameComponent = &g_mgc;
static int g_freed = 0;
static void TrackedFree(void* p) { ++g_freed; free(p); }
namespace NHeroInformationScreens {
class CBase { public: EPlayerMode Mode; CBase(EPlayerMode m) : Mode(m) { } virtual ~CBase() { } virtual EPlayerMode GetPlayerMode() const { return Mode; } };
template <class T> class CVector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVector() { if (_M_start) TrackedFree(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
class CManager {
public:
    CWorld* World;
    CVector<const CBase*> Screens;
    void GetAllRegisteredScreens(CVector<const CBase*>& screens) const
    {
        screens._M_start = (const CBase**)malloc(Screens.size() * sizeof(const CBase*) + 4);
        screens._M_finish = screens._M_start;
        for (unsigned int i = 0; i < Screens.size(); ++i) if (Screens._M_start[i]) *screens._M_finish++ = Screens._M_start[i];
        screens._M_end_of_storage = screens._M_finish;
    }
    void RemoveAllPlayerModes();
};
void CManager::RemoveAllPlayerModes()
{
    CPlayer* player = g_pMainGameComponent->PPlayerManager->GetMainPlayer();
    unsigned int i = 0;
    CVector<const CBase*> screens;
    GetAllRegisteredScreens(screens);
    const unsigned int count = screens.size();
    const CBase** first = screens._M_start;
    for (; i < count; ++i)
        player->RemoveMode(first[i]->GetPlayerMode());
}
}
int main()
{
    using namespace NHeroInformationScreens;
    g_mgc.PPlayerManager = &g_pm; g_player = &g_mainPlayer;
    CBase a(PLAYER_MODE_A), b(PLAYER_MODE_B), c(PLAYER_MODE_C);
    const CBase* table[5] = { &a, 0, &b, 0, &c };
    CManager m; m.World = 0;
    m.Screens._M_start = table; m.Screens._M_finish = table + 5; m.Screens._M_end_of_storage = table + 5;
    m.RemoveAllPlayerModes();
    bool ok = g_removedCount == 3 && g_removed[0] == PLAYER_MODE_A && g_removed[1] == PLAYER_MODE_B && g_removed[2] == PLAYER_MODE_C && g_freed == 1;
    m.Screens._M_finish = table;
    m.RemoveAllPlayerModes();
    ok = ok && g_removedCount == 3 && g_freed == 2;
    m.Screens._M_start = m.Screens._M_finish = m.Screens._M_end_of_storage = 0;
    if (!ok) { printf("0066e985_TEST FAIL\n"); return 1; }
    printf("0066e985_TEST PASS\n");
    return 0;
}
