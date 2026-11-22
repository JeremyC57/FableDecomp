// Test for 0x00496CF0 CFrontendGameComponent::Run(CGameComponent**) on the shared headers (NF236, replaces the HEAD host model): the
// row's defines, includes, local shapes and body, verbatim. Stubbed (recording): GFGetSystemManager / CSystemManager::Update (a
// scripted result list), Input / Output / Shutdown, the CNewFrontendGameComponent / CMainGameComponent ctors (they record their init and
// game; their vtables' slots are empty stubs), CGameComponent::Init (the new component's slot-1 call), CGameJoystickManager::Get /
// SetMainPlayerJoystickDeviceNumber, CStopWatch's ctor, CWideString / CCharString ctors, dtors and assignment; the new components'
// other virtuals are empty (CNewFrontendGameComponent::Init, its slot-1 override, counts like CGameComponent::Init). The frontend is a zeroed buffer of the class
// size with its fields seeded at the CTOR_DTOR offsets (Quit +8, Running +9, Game +0x0C, CreateGame +0x10, joystick +0x14,
// CreateReleaseFrontEnd +0x19, SaveToLoad +0x1E2C, WorldToLoad +0x1E40), run through a qualified (non-virtual) call.
// Case 1 (CreateGame): updates 1, 0, 1, 2 give two Input / Output frames, then quit; Running is cleared; Shutdown once; the main-game
//   init gets WorldToLoad and SaveToLoad (assignment sources), the joystick number is passed, the new component's Init runs, *next is
//   it, the result is true and GOldGameComponentToDelete is the frontend.
// Case 2 (neither flag): an immediate 2 gives no frames, Shutdown once, *next untouched, false, GOldGameComponentToDelete the frontend.
// Case 3 (CreateReleaseFrontEnd and CreateGame): the release front end wins: a new CNewFrontendGameComponent on the same game.
// Case 4: with Quit already set the loop body never runs (no Update call).
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CSTOPWATCH_CTOR
#define FABLE_RETAIL_CGAMEJOYSTICKMANAGER_MAINPLAYERJOYSTICK
#define FABLE_RETAIL_CMAINGAMECOMPONENTINIT_INLINE_CTOR_DTOR
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTexture.h"
// Naming (NF129, 2026-11-20, q4289): CSystemManager::Update returns ESystemReturn (Ego_r.pdb 0x00AF4520 + FableWin.pdb 0x02F8AB00
//   ?Update@CSystemManager@@QAE?AW4ESystemReturn@@XZ; GFPlayFullScreenXBoxMovie 0x006286F0 binds it); respelled in place in the local
//   CSystemManager model, the enum from engine/CSystemManager.h's partial mode FABLE_RETAIL_CSYSTEMMANAGER_ESYSTEMRETURN_ONLY (the full
//   header needs the Win32 types; the HEAD local model stays, in place).
#define FABLE_RETAIL_CSYSTEMMANAGER_ESYSTEMRETURN_ONLY
#include "engine/CSystemManager.h"
class CSystemManager { public: ESystemReturn Update(); };                 // 0x009A6460
CSystemManager* __fastcall GFGetSystemManager();                        // 0x009A4EC0
#include "engine/CGameJoystickManager.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/CMainGameComponent.h"
#include "engine/CFrontendGameComponent.h"

class CNewFrontendGameComponentInit { public: long Nothing; CNewFrontendGameComponentInit() : Nothing(0) { } };
extern CGameComponent* GOldGameComponentToDelete;        // 0x013B7D58
CGameComponent* GOldGameComponentToDelete = 0;
static int g_script[8]; static int g_pos = 0, g_updates = 0, g_inputs = 0, g_outputs = 0, g_shutdowns = 0, g_inits = 0;
static int g_newFrontends = 0, g_newMains = 0; static long g_joystick = -1; static CGame* g_newGame = 0;
static const void* g_assignDst[4]; static const void* g_assignSrc[4]; static int g_assigns = 0;
static const void* g_mainInit = 0;
static unsigned char g_sys[4];
CSystemManager* __fastcall GFGetSystemManager() { return (CSystemManager*)g_sys; }
ESystemReturn CSystemManager::Update() { ++g_updates; return (ESystemReturn)g_script[g_pos++]; }
void CFrontendGameComponent::Input() { ++g_inputs; }
void CFrontendGameComponent::Output() { ++g_outputs; }
void CFrontendGameComponent::Shutdown() { ++g_shutdowns; }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CGameComponent::CGameComponent(CGame& game) : Quit(false), Running(false), Game(game) {}
CGameComponent::~CGameComponent() {}
void CGameComponent::Init() { ++g_inits; }
void CGameComponent::SetQuit() {}
CNewFrontendGameComponent::CNewFrontendGameComponent(CGame& game, const CNewFrontendGameComponentInit&) : CGameComponent(game) { ++g_newFrontends; g_newGame = &game; }
CMainGameComponent::CMainGameComponent(CGame& game, const CMainGameComponentInit& init) : CGameComponent(game) { ++g_newMains; g_newGame = &game; g_mainInit = &init; }
CStopWatch::CStopWatch() {}
CNewFrontendGameComponent::~CNewFrontendGameComponent() {}
void CNewFrontendGameComponent::Init() { ++g_inits; }
bool CNewFrontendGameComponent::Run(CGameComponent**) { return false; }
void CNewFrontendGameComponent::ChangeTextureColourDepth(long) {}
void CNewFrontendGameComponent::OnPreDeviceReset() {}
bool CNewFrontendGameComponent::OnPostDeviceReset() { return true; }
CMainGameComponent::~CMainGameComponent() {}
bool CMainGameComponent::Run(CGameComponent**) { return false; }
void CMainGameComponent::ChangeTextureColourDepth(long) {}
void CMainGameComponent::OnPreDeviceReset() {}
bool CMainGameComponent::OnPostDeviceReset() { return true; }
static unsigned char g_gjm[4];
CGameJoystickManager& __fastcall CGameJoystickManager::Get() { return *(CGameJoystickManager*)g_gjm; }
void CGameJoystickManager::SetMainPlayerJoystickDeviceNumber(long number) { g_joystick = number; }
long CWideString::NoStrings = 0;
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() {}
const CWideString& CWideString::operator=(const CWideString& other) { if (g_assigns < 4) { g_assignDst[g_assigns] = this; g_assignSrc[g_assigns] = &other; ++g_assigns; } return *this; }
CCharString::CCharString() {}
CCharString::~CCharString() {}
bool CFrontendGameComponent::Run(CGameComponent** nextComponent)
{
    Running = true;
    while (!Quit) {
        long state = GFGetSystemManager()->Update();
        if (state == 2)
            Quit = true;
        else if (state == 1) {
            Input();
            Output();
        }
    }
    Running = false;
    if (CreateReleaseFrontEnd) {
        Shutdown();
        CNewFrontendGameComponentInit init;
        CGameComponent* component = new CNewFrontendGameComponent(Game, init);
        component->Init();
        *nextComponent = component;
        GOldGameComponentToDelete = this;
        return true;
    }
    if (CreateGame) {
        Shutdown();
        CMainGameComponentInit init;
        init.InitialWorldName = WorldToLoad;
        init.SaveGameName = SaveToLoad;
        CGameJoystickManager::Get().SetMainPlayerJoystickDeviceNumber(CreateGameJoystickDeviceNumber);
        CGameComponent* component = new CMainGameComponent(Game, init);
        component->Init();
        *nextComponent = component;
        GOldGameComponentToDelete = this;
        return true;
    }
    Shutdown();
    GOldGameComponentToDelete = this;
    return false;
}
static double g_raw[sizeof(CFrontendGameComponent) / sizeof(double) + 1];
static unsigned char* B() { return (unsigned char*)g_raw; }
static void Reset(CGame* game, bool createGame, bool release, long joystick, const int* script, int n)
{
    std::memset(g_raw, 0, sizeof(g_raw));
    std::memcpy(B() + 0x0C, &game, 4);
    B()[0x10] = createGame; B()[0x19] = release; std::memcpy(B() + 0x14, &joystick, 4);
    for (int i = 0; i < n; ++i) g_script[i] = script[i];
    g_pos = g_updates = g_inputs = g_outputs = g_shutdowns = g_inits = g_newFrontends = g_newMains = g_assigns = 0;
    g_joystick = -1; g_newGame = 0; g_mainInit = 0; GOldGameComponentToDelete = 0;
}
int main()
{
    int fails = 0;
    static unsigned char gameBuf[16]; CGame* game = (CGame*)gameBuf;
    CFrontendGameComponent* c = (CFrontendGameComponent*)g_raw;
    CGameComponent* next = 0;
    static const int s1[] = { 1, 0, 1, 2 };
    Reset(game, true, false, 3, s1, 4);
    bool r = c->CFrontendGameComponent::Run(&next);
    if (!r || g_updates != 4 || g_inputs != 2 || g_outputs != 2 || B()[8] != 1 || B()[9] != 0 || g_shutdowns != 1) fails |= 1;
    if (g_newMains != 1 || g_newFrontends != 0 || g_newGame != game || g_joystick != 3 || g_inits != 1 || next == 0) fails |= 1;
    if (g_assigns != 2 || g_assignSrc[0] != B() + 0x1E40 || g_assignSrc[1] != B() + 0x1E2C) fails |= 1;
    if (g_mainInit == 0 || g_assignDst[0] != (const char*)g_mainInit + 0x00 || g_assignDst[1] != (const char*)g_mainInit + 0x0C) fails |= 1;
    if (GOldGameComponentToDelete != (CGameComponent*)c) fails |= 1;
    static const int s2[] = { 2 };
    Reset(game, false, false, 0, s2, 1); next = 0;
    r = c->CFrontendGameComponent::Run(&next);
    if (r || g_updates != 1 || g_inputs != 0 || g_shutdowns != 1 || next != 0 || g_newMains + g_newFrontends != 0 || g_inits != 0) fails |= 2;
    if (GOldGameComponentToDelete != (CGameComponent*)c || B()[9] != 0) fails |= 2;
    static const int s3[] = { 0, 2 };
    Reset(game, true, true, 5, s3, 2); next = 0;
    r = c->CFrontendGameComponent::Run(&next);
    if (!r || g_newFrontends != 1 || g_newMains != 0 || g_newGame != game || g_inits != 1 || next == 0 || g_joystick != -1 || g_shutdowns != 1) fails |= 4;
    if (GOldGameComponentToDelete != (CGameComponent*)c) fails |= 4;
    Reset(game, false, false, 0, s2, 1); B()[8] = 1;
    r = c->CFrontendGameComponent::Run(&next);
    if (r || g_updates != 0 || g_shutdowns != 1) fails |= 8;
    if (fails) { std::printf("00496cf0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00496cf0_TEST PASS\n");
    return 0;
}
