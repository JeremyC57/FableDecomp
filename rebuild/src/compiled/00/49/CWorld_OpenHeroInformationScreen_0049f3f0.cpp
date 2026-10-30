// CWorld::OpenHeroInformationScreen(EPlayerMode mode) -- takes the main player out of every registered hero-information-screen mode, puts it into `mode`, opens the hero information screens and pauses the world
// Leaf: ?OpenHeroInformationScreen@CWorld@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049F3F0, 194 bytes (padded to 208), `ret 4`.
//   PDB name ?OpenHeroInformationScreen@CWorld@@QAEXW4EPlayerMode@@@Z.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (PlayerManager +0xc); CPlayerGui::GetPHeroInfoScreensManager
//   0x00437CD0 on g_pPlayerGui 0x013B8790 (twice); NHeroInformationScreens::CManager::GetAllRegisteredScreens 0x0066E941
//   and Open 0x0066EA46; each screen's virtual slot 1 (+4, its player mode); CPlayer::RemoveMode(EPlayerMode)
//   0x00634190 and AddMode(EPlayerMode, bool) 0x006345C0; CRT free 0x00BFEA14 (the vector's storage).
// Shape: the screen manager is fetched into a local before the vector is declared; the screens vector is a default-constructed STLport vector local (three zero stores after the argument
//   push), its size re-read every pass (its address escaped); the pause is the inline SetAsPaused(true)
//   (`if (Paused != paused) Paused = paused;`); the vector's inline destructor frees the storage.
// Layout (engine/CWorld.h, below +0xac retail = PDB): PlayerManager +0x0c, Paused +0xa4.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
enum EPlayerMode { PLAYER_MODE_DUMMY = 0 };
namespace NHeroInformationScreens {
class CBase { public:
    virtual ~CBase();
    virtual EPlayerMode GetPlayerMode() const;     // slot 1 (+4)
};
}
template <class T> struct CRetailVector {          // STLport vector (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
namespace NHeroInformationScreens {
class CManager { public:
    void GetAllRegisteredScreens(CRetailVector<const CBase*>* screens);   // 0x0066E941
    void Open();                                   // 0x0066EA46
};
}
class CPlayerGui { public:
public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
class CPlayer { public:
    void RemoveMode(EPlayerMode mode);             // 0x00634190
    void AddMode(EPlayerMode mode, bool flag);     // 0x006345C0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorld { public:
    void*           __vftable;                     // +0x00
    unsigned char   _base04[4];                    // +0x04
    void*           Component;                     // +0x08
    CPlayerManager* PlayerManager;                 // +0x0c
    unsigned char   _pad10[0xa4 - 0x10];
    bool            Paused;                        // +0xa4
    void SetAsPaused(bool paused)
    {
        if (Paused != paused)
            Paused = paused;
    }
    void OpenHeroInformationScreen(EPlayerMode mode);
};
void CWorld::OpenHeroInformationScreen(EPlayerMode mode)
{
    CPlayer* player = &PlayerManager->GetMainPlayer();
    NHeroInformationScreens::CManager* manager = g_pPlayerGui->GetPHeroInfoScreensManager();
    CRetailVector<const NHeroInformationScreens::CBase*> screens;
    manager->GetAllRegisteredScreens(&screens);
    for (unsigned long i = 0; i < screens.size(); ++i)
        player->RemoveMode(screens[i]->GetPlayerMode());
    player->AddMode(mode, false);
    g_pPlayerGui->GetPHeroInfoScreensManager()->Open();
    SetAsPaused(true);
}