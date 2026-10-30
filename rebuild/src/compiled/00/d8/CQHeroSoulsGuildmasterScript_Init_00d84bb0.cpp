// CQ_HeroSoulsGuildmasterScript::Init() -- reset the cut-scene flags (warning cut-scene on) and the guard counters, then register the quest region ("Q_HeroSoulsGuildmaster", "GuildWoods")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D84BB0, length 106 bytes (padded to 112).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2: "GuildWoods" @ 0x012CD7DC,
// "Q_HeroSoulsGuildmaster" @ 0x012D30EC), CGameScriptInterface::AddQuestRegion (slot 271, +0x43c) on the script's own
// Game (+0x40), CCharString::~CCharString @ 0x0099EAE0 (x2).
// Layout (retail = Ego_r PDB offsets -0x14): PlayedBriarCS +0x48, PlayedGuildMasterCS +0x49, PlayedGuildCS +0x4a,
// OutsideWoodGuardsAttack +0x4b, GiveWarningCutscene +0x4c, GuardInWoodsCount +0x50, DistanceReached +0x54.
// Shape (landed 0x00E1A870 form): one zero register for the member stores, scheduled into the first string's argument
// pushes in source order (+0x49 first); both literals are implicit conversion temporaries built right to left.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
struct CQ_HeroSoulsGuildmasterScript {
    unsigned char         _pad_0x00[0x40];
    CGameScriptInterface* Game;                       // +0x40
    unsigned char         _pad_0x44[4];
    bool                  PlayedBriarCS;              // +0x48
    bool                  PlayedGuildMasterCS;        // +0x49
    bool                  PlayedGuildCS;              // +0x4a
    bool                  OutsideWoodGuardsAttack;    // +0x4b
    bool                  GiveWarningCutscene;        // +0x4c
    long                  GuardInWoodsCount;          // +0x50
    long                  DistanceReached;            // +0x54
    void Init();
};
void CQ_HeroSoulsGuildmasterScript::Init()
{
    PlayedGuildMasterCS = false;
    PlayedBriarCS = false;
    PlayedGuildCS = false;
    OutsideWoodGuardsAttack = false;
    GiveWarningCutscene = true;
    GuardInWoodsCount = 0;
    DistanceReached = 0;
    Game->AddQuestRegion("Q_HeroSoulsGuildmaster", "GuildWoods");
}