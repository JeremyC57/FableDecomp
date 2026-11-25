// Behaviour test for SavedConfirmed @ 0x0047DB73 (NF47 round 7): the row now reads ?PMainGameComponent@CMainGameComponent@@1PAV1@A
// through engine/CMainGameComponent.h and calls ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ and
// ?GetPPlayerCharacterThing@CPlayer@@QBEPAVCThing@@XZ (engine/CPlayerManager.h / engine/CPlayer.h), so the test defines those symbols
// as recording doubles over opaque storage. D / GlobalGui are the row's own remaining stand-ins (unchanged doubles).
// NF200 (2026-11-20): the Wrapper::Check stand-in is gone; the row calls the shared IS_THING_ALIVE<CThing*> (engine/ThingAlive.h, its
// pick-any COMDAT is in the row object), which reads CThing::Dead, so the thing store is a real CThing-sized buffer and the test flips
// the Dead bit (+0x91 bit 0, engine/CThing.h) instead of a Wrapper double.
// NF209 (2026-11-20, q11772): the D::Do stand-in is gone; the row calls std::bitset<274>::test (declared only under
// FABLE_RETAIL_STLP_BITSET_TEST_DECL), so the test defines that member as the recording double.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"

#include <cstdio>

struct GlobalGui {
};
// Naming (NF442, ruling q2): the row now calls the callee by its PDB spelling, so this double is respelt with it (callee only).
class CPlayerGui { public: void ToggleLiveGui(); };

GlobalGui* g_pGlobalGui;
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;

static unsigned char g_mainStore[0x40];
static unsigned char g_managerStore[0x40];
static unsigned char g_playerStore[0x40];
static unsigned char g_thingStore[sizeof(CThing)];

static const void* g_manager_seen = 0;
static const void* g_player_seen  = 0;
static int  g_do_called   = 0;
static int  g_do_arg      = 0;
static const void* g_do_this = 0;
static int  g_notify      = 0;

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
namespace std { template<> bool bitset<274>::test(size_t n) const { g_do_called++; g_do_arg = (int)n; g_do_this = this; return false; } }
void CPlayerGui::ToggleLiveGui() { g_notify++; }

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

    g_thingStore[0x91] = 0;
    SavedConfirmed();

    if (g_manager_seen != g_managerStore)  { std::printf("FAIL manager_seen\n"); return 1; }
    if (g_player_seen != g_playerStore)    { std::printf("FAIL player_seen\n"); return 1; }
    if (g_do_called != 1)                  { std::printf("FAIL do_called\n"); return 1; }
    if (g_do_arg != 0xC6)                  { std::printf("FAIL do_arg %d\n", g_do_arg); return 1; }
    if (g_do_this != g_thingStore + 0x20)  { std::printf("FAIL do_this\n"); return 1; }
    if (g_notify != 1)                     { std::printf("FAIL notify\n"); return 1; }

    g_thingStore[0x91] = 1;
    g_do_called  = 0;
    g_notify     = 0;

    SavedConfirmed();

    if (g_do_called != 0) { std::printf("FAIL inactive do\n"); return 1; }
    if (g_notify != 0)    { std::printf("FAIL inactive notify\n"); return 1; }

    std::printf("SAVEDCONFIRMED_0047DB73_TEST PASS\n");
    return 0;
}
