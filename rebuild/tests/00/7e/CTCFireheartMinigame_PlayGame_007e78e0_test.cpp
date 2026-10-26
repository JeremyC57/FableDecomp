#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CTCBase;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    CCharString() : PStringData(0) {}               // test only: the global table
    ~CCharString();                                 // 0x0099EAE0
};
extern CCharString g_FireheartMarkerNames[4][4];    // 0x013BAEB0

class CThingFilter_HasScriptName {
public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};

class CThingSearchTools {
public:
    template<class F, class T> bool GetPFirstThing(const F& filter, T** ppThing) const;   // 0x0056D050 (HasScriptName, CThing)
};

class C3DVector;
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& position);   // slot 30 (+0x78)
};

class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

class CPlayer {
public:
    long GetCurrentMode();                          // 0x00633BE0 (EPlayerMode)
    void AddMode(long mode, bool flag);             // 0x006345C0
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                       // 0x00449970
};

class CPlayerGui {
public:
    unsigned char _pad00[0x330];
    bool DisplayBottomHalfGUI;                      // +0x330
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];
    CThingSearchTools* GetThingSearchTools() const; // 0x00686D90
    CPlayerManager* GetPlayerManager_00686d40();             // 0x00686D40
};

class CTCFireheartMinigame : public CTCBase {
public:
    CThing* Markers[4][4];                          // +0x0C
    bool InSession;                                 // +0x4C
    bool ForceQuit;                                 // +0x4D
    unsigned char _pad4e[2];
    CThing* CurrentMarker;                          // +0x50
    bool Initialised;                               // +0x54
    bool BlocksActive[4][4];                        // +0x55
    long XPos;                                      // +0x68
    long YPos;                                      // +0x6C
    bool Moving;                                    // +0x70
    unsigned char _pad71[0x88 - 0x71];
    bool QuitWithDelay;                             // +0x88
    void PlayGame();
};

void CTCFireheartMinigame::PlayGame()
{
    if (!Initialised) {
        for (long row = 0; row < 4; ++row)
            for (long column = 0; column < 4; ++column)
                GetThingSearchTools()->GetPFirstThing(CThingFilter_HasScriptName(g_FireheartMarkerNames[row][column]),
                                                      &Markers[row][column]);
    }
    InSession = true;
    ForceQuit = false;
    XPos = 0;
    YPos = 3;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    physics->SetPosition(Markers[0][3]->GetPosition());
    CPlayer* player = GetPlayerManager_00686d40()->GetMainPlayer();
    if (player->GetCurrentMode() != 0x2a)
        player->AddMode(0x2a, false);
    g_pPlayerGui->DisplayBottomHalfGUI = false;
    QuitWithDelay = false;
}

// ---- test stubs for the retail callees ----
static int g_copies, g_dtors, g_finds, g_added; static long g_addedMode; static long g_mode;
static const C3DVector* g_setPos;
static CThing g_things[16];
CCharString::CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
CCharString g_FireheartMarkerNames[4][4];
template<class F, class T> bool CThingSearchTools::GetPFirstThing(const F& filter, T** ppThing) const
{ long i = (long)(size_t)filter.ScriptName.PStringData; *ppThing = &g_things[i]; ++g_finds; return true; }
class C3DVector { public: float X, Y, Z; };
static C3DVector g_positions[16];
const C3DVector& CThing::GetPosition() const { return g_positions[this - g_things]; }
struct CTestPhysics : CTCPhysicsBase { virtual void SetPosition(const C3DVector& p) { g_setPos = &p; } };
#define V(n) void CTCPhysicsBase::n() {}
V(v00) V(v01) V(v02) V(v03) V(v04) V(v05) V(v06) V(v07) V(v08) V(v09) V(v10) V(v11) V(v12) V(v13) V(v14)
V(v15) V(v16) V(v17) V(v18) V(v19) V(v20) V(v21) V(v22) V(v23) V(v24) V(v25) V(v26) V(v27) V(v28) V(v29)
void CTCPhysicsBase::SetPosition(const C3DVector&) {}
long CPlayer::GetCurrentMode() { return g_mode; }
void CPlayer::AddMode(long mode, bool flag) { ++g_added; g_addedMode = flag ? -1 : mode; }
static CPlayer g_player; static CPlayerManager g_pm; static CThingSearchTools g_tools;
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
void CTCBase::v00() {}
CThingSearchTools* CTCBase::GetThingSearchTools() const { return &g_tools; }
CPlayerManager* CTCBase::GetPlayerManager_00686d40() { return &g_pm; }

int main()
{
    int ok = 1;
    for (long i = 0; i < 16; ++i) g_FireheartMarkerNames[i / 4][i % 4].PStringData = (void*)(size_t)i;
    CTestPhysics physics; CThing owner; owner.PhysicsTC = &physics;
    CTCFireheartMinigame game; std::memset(game.Markers, 0, sizeof(game.Markers));
    game.ParentThing = &owner; game.Initialised = false; game.InSession = false; game.ForceQuit = true;
    game.XPos = 7; game.YPos = 7; game.QuitWithDelay = true; g_gui.DisplayBottomHalfGUI = true;
    g_mode = 5; g_copies = g_dtors = 0;
    game.PlayGame();
    for (long r = 0; r < 4; ++r) for (long c = 0; c < 4; ++c) if (game.Markers[r][c] != &g_things[r * 4 + c]) ok = 0;
    if (g_finds != 16 || g_copies != 16 || g_dtors != 16) ok = 0;
    if (!game.InSession || game.ForceQuit || game.XPos != 0 || game.YPos != 3 || game.QuitWithDelay) ok = 0;
    if (g_setPos != &g_positions[3] || g_added != 1 || g_addedMode != 0x2a || g_gui.DisplayBottomHalfGUI) ok = 0;
    // initialised and already in the mode: no searches, no AddMode
    game.Initialised = true; g_mode = 0x2a;
    game.PlayGame();
    if (g_finds != 16 || g_added != 1) ok = 0;
    if (!ok) { std::printf("007e78e0_TEST FAIL\n"); return 1; }
    std::printf("007e78e0_TEST PASS\n");
    return 0;
}