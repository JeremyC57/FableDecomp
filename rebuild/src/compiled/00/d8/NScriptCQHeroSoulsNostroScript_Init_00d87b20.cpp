// CQ_HeroSoulsNostroScript::Init() -- reset the summoner/undead counters and flags and the vulnerable timer, then register the quest region ("Q_HeroSoulsNostro", "GraveyardCircle")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D87B20, length 115 bytes (padded to 128).
// Callees: CGameScriptInterface::SetTimer (slot 89, +0x164) through the global @ 0x0143E8F8,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2: "GraveyardCircle" @ 0x012D49A8, "Q_HeroSoulsNostro"
// @ 0x012D30D8), CGameScriptInterface::AddQuestRegion (slot 271, +0x43c) on the script's own Game (+0x40),
// CCharString::~CCharString @ 0x0099EAE0 (x2).
// Layout (retail = Ego_r PDB offsets -0x14): SummonerCount +0x48, PlayedNostroCS +0x4c, KickedOffQuestStartScreen +0x4d,
// UndeadAlive +0x50, NostroVulnerableTimer +0x54.
// Shape (landed 0x00E1A870 form): one zero register for the member stores and the timer's 0 argument; both literals are
// implicit conversion temporaries built right to left in an 8-byte frame, destroyed after the call in reverse order.
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
class CQ_HeroSoulsNostroScript {
public:
    unsigned char         _pad_0x00[0x40];
    CGameScriptInterface* Game;                       // +0x40
    unsigned char         _pad_0x44[4];
    long                  SummonerCount;              // +0x48
    bool                  PlayedNostroCS;             // +0x4c
    bool                  KickedOffQuestStartScreen;  // +0x4d
    long                  UndeadAlive;                // +0x50
    CTimer                NostroVulnerableTimer;      // +0x54
    void Init();
};
} // namespace NScript
using NScript::CQ_HeroSoulsNostroScript;
void NScript::CQ_HeroSoulsNostroScript::Init()
{
    SummonerCount = 0;
    PlayedNostroCS = false;
    KickedOffQuestStartScreen = false;
    UndeadAlive = 0;
    NostroVulnerableTimer = 0;
    Game->AddQuestRegion("Q_HeroSoulsNostro", "GraveyardCircle");
}