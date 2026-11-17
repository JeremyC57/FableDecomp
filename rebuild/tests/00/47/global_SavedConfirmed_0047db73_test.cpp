// Behaviour test for SavedConfirmed @ 0x0047DB73 (NF47 round 7): the row now reads ?PMainGameComponent@CMainGameComponent@@1PAV1@A
// through engine/CMainGameComponent.h and calls ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ and
// ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ (engine/CPlayerManager.h / engine/CPlayer.h), so the test defines those symbols
// as recording doubles over opaque storage. Wrapper / D / GlobalGui are the row's own remaining stand-ins (unchanged doubles).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#include <cstdio>

struct D {
    void Do(int arg);
};

struct Wrapper {
    CThing* p;
    bool Check();
};

struct GlobalGui {
    void Notify();
};

GlobalGui* g_pGlobalGui;
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;

static unsigned char g_mainStore[0x40];
static unsigned char g_managerStore[0x40];
static unsigned char g_playerStore[0x40];
static unsigned char g_thingStore[0x40];

static const void* g_manager_seen = 0;
static const void* g_player_seen  = 0;
static int  g_do_called   = 0;
static int  g_do_arg      = 0;
static const void* g_do_this = 0;
static int  g_notify      = 0;
static bool g_check_ret   = true;
static CThing* g_check_seen = 0;

const CPlayer& CPlayerManager::GetMainPlayer() const
{
    g_manager_seen = this;
    return *reinterpret_cast<const CPlayer*>(g_playerStore);
}
CThing* CPlayer::GetPPlayerCharacterThing() const
{
    g_player_seen = this;
    return reinterpret_cast<CThing*>(g_thingStore);
}
void D::Do(int a)        { g_do_called++; g_do_arg = a; g_do_this = this; }
void GlobalGui::Notify() { g_notify++; }
bool Wrapper::Check()    { g_check_seen = p; return g_check_ret; }

void __fastcall SavedConfirmed();

// PMainGameComponent is protected: its address is taken inside an explicit instantiation, where access is not checked
// (C++03 14.7.2/8), and handed out by the instantiated static member's constructor.
static CMainGameComponent** g_ppMain = 0;
template <CMainGameComponent** P> struct GrabMainStatic {
    GrabMainStatic() { g_ppMain = P; }
    static GrabMainStatic inst;
};
template <CMainGameComponent** P> GrabMainStatic<P> GrabMainStatic<P>::inst;
template struct GrabMainStatic<&CMainGameComponent::PMainGameComponent>;

int main()
{
    static GlobalGui theGui;
    g_pGlobalGui = &theGui;
    if (!g_ppMain) { std::printf("FAIL static address\n"); return 1; }
    *reinterpret_cast<void**>(g_mainStore + 0x1C) = g_managerStore;   // PPlayerManager: the scoped_ptr word +0x1C
    *g_ppMain = reinterpret_cast<CMainGameComponent*>(g_mainStore);
    if (CMainGameComponent::Get() != reinterpret_cast<CMainGameComponent*>(g_mainStore)) { std::printf("FAIL Get\n"); return 1; }

    g_check_ret  = true;
    SavedConfirmed();

    if (g_manager_seen != g_managerStore)  { std::printf("FAIL manager_seen\n"); return 1; }
    if (g_player_seen != g_playerStore)    { std::printf("FAIL player_seen\n"); return 1; }
    if (g_check_seen != reinterpret_cast<CThing*>(g_thingStore)) { std::printf("FAIL check_seen\n"); return 1; }
    if (g_do_called != 1)                  { std::printf("FAIL do_called\n"); return 1; }
    if (g_do_arg != 0xC6)                  { std::printf("FAIL do_arg %d\n", g_do_arg); return 1; }
    if (g_do_this != g_thingStore + 0x20)  { std::printf("FAIL do_this\n"); return 1; }
    if (g_notify != 1)                     { std::printf("FAIL notify\n"); return 1; }

    g_check_ret  = false;
    g_do_called  = 0;
    g_notify     = 0;

    SavedConfirmed();

    if (g_do_called != 0) { std::printf("FAIL inactive do\n"); return 1; }
    if (g_notify != 0)    { std::printf("FAIL inactive notify\n"); return 1; }

    std::printf("SAVEDCONFIRMED_0047DB73_TEST PASS\n");
    return 0;
}
