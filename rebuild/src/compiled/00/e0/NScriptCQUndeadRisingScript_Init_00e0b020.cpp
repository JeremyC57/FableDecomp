// CQ_UndeadRisingScript::Init() -- register the quest region ("Q_UndeadRising", "Graveyard"), then reset the token/kill counters, the quest status, the four mission flags and the eight light flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0B020, length 115 bytes (padded to 128).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2: "Graveyard" @ 0x012D49B8,
// "Q_UndeadRising" @ 0x012DA210), CGameScriptInterface::AddQuestRegion (slot 271, +0x43c) on the script's own Game
// (+0x40), CCharString::~CCharString @ 0x0099EAE0 (x2).
// Layout (retail = Ego_r PDB offsets -0x14): number_of_zombie_tokens_in_right_area +0x48, number_of_boss_kills_in_right_area
// +0x4c, HeroReachedCentre/BossCutscenePlayed/MissionSucceeded/MissionFailed +0x50..+0x53, LightsActivated[8] +0x54,
// quest_status +0x5c.
// Shape (landed 0x00E1A870 form): the member stores follow the string destructors; the light flags are cleared as two
// zero dwords through `lea eax,[esi+54h]` with their own zero register: that is VC7.1's expansion of a plain
// element `for` loop (the memset intrinsic stores through esi with the shared zero register instead).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
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
namespace NScript {
class CQ_UndeadRisingScript {
public:
    unsigned char         _pad_0x00[0x40];
    CGameScriptInterface* Game;                                  // +0x40
    unsigned char         _pad_0x44[4];
    unsigned long         number_of_zombie_tokens_in_right_area; // +0x48
    unsigned long         number_of_boss_kills_in_right_area;    // +0x4c
    bool                  HeroReachedCentre;                     // +0x50
    bool                  BossCutscenePlayed;                    // +0x51
    bool                  MissionSucceeded;                      // +0x52
    bool                  MissionFailed;                         // +0x53
    bool                  LightsActivated[8];                    // +0x54
    long                  quest_status;                          // +0x5c
    void Init();
};
} // namespace NScript
using NScript::CQ_UndeadRisingScript;
void NScript::CQ_UndeadRisingScript::Init()
{
    Game->AddQuestRegion("Q_UndeadRising", "Graveyard");
    number_of_zombie_tokens_in_right_area = 0;
    number_of_boss_kills_in_right_area = 0;
    quest_status = 0;
    HeroReachedCentre = false;
    BossCutscenePlayed = false;
    MissionSucceeded = false;
    MissionFailed = false;
    for (int i = 0; i < 8; ++i)
        LightsActivated[i] = false;
}