// CQ_HeroSoulsBriarScript::Init() -- reset the quest flags and both imposter timers, then register the quest region ("Q_HeroSoulsBriar", "NorthernWastes3")
// Shape note: CTimer::Set reads the index through a const reference (`const long& index = Index;`): VC7.1 then keeps
// the index load after the PlayedBriarCS byte store and the interface load (retail order); the plain member read is hoisted.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80CC0, length 130 bytes (padded to 144).
// Callees: CGameScriptInterface::SetTimer (slot 89, +0x164) through the global @ 0x0143E8F8 (x2),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2: "NorthernWastes3" @ 0x012C4538, "Q_HeroSoulsBriar"
// @ 0x012D31AC), CGameScriptInterface::AddQuestRegion (slot 271, +0x43c) on the script's own Game (+0x40),
// CCharString::~CCharString @ 0x0099EAE0 (x2).
// Vtable 0x012D41BC slot 3. Layout (retail; PDB offsets -0x14): PlayedBriarCS +0x48, the two timers +0x68/+0x6c,
// KickOffQuestStartScreen +0x70.
// Shape (landed 0x00E1A870 form): both literals are implicit conversion temporaries built right to left (region first)
// in an 8-byte frame and destroyed after the call in reverse order; the KickOff byte store is scheduled into the first
// string's argument pushes.
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
inline CTimer& CTimer::operator=(long time) { const long& index = TimerIndex; g_pGameScriptInterface->SetTimer(index, time); return *this; }
namespace NScript {
class CQ_HeroSoulsBriarScript {
public:
    unsigned char         _pad_0x00[0x40];
    CGameScriptInterface* Game;                       // +0x40
    unsigned char         _pad_0x44[4];
    bool                  PlayedBriarCS;              // +0x48
    unsigned char         _pad_0x49[0x68 - 0x49];     // seh_BriarRose +0x4c, csargs +0x5c
    CTimer                BriarRoseImposterTauntTimer; // +0x68
    CTimer                BriarRoseImposterHereTimer;  // +0x6c
    bool                  KickOffQuestStartScreen;     // +0x70
    void Init();
};
} // namespace NScript
using NScript::CQ_HeroSoulsBriarScript;
void NScript::CQ_HeroSoulsBriarScript::Init()
{
    PlayedBriarCS = false;
    BriarRoseImposterTauntTimer = 0;
    BriarRoseImposterHereTimer = 0;
    KickOffQuestStartScreen = false;
    Game->AddQuestRegion("Q_HeroSoulsBriar", "NorthernWastes3");
}