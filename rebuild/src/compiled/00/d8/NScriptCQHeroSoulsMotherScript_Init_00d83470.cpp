// CQ_HeroSoulsMotherScript::Init() -- reset the cut-scene flag, bar and screamer counters, then register the quest region ("Q_HeroSoulsMother", "OakvaleMemorialGarden")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D83470, length 102 bytes (padded to 112).
// Label correction: the catalog calls this CQ_HeroSoulsGuildmasterScript::Init; it is slot 3 of vtable 0x012D45F0, the
// vtable the CQ_HeroSoulsMotherScript ctor (0x00D83400) stores, and it registers "Q_HeroSoulsMother".
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x2: "OakvaleMemorialGarden" @ 0x012C4824,
// "Q_HeroSoulsMother" @ 0x012D3210), CGameScriptInterface::AddQuestRegion (slot 271, +0x43c) on the script's own Game
// (+0x40), CCharString::~CCharString @ 0x0099EAE0 (x2).
// Layout (retail; PDB offsets -0x14, the retail STLport map is 12 bytes): PlayedMotherCS +0x48, ScreamersAlive +0x58,
// BarToFill +0x5c, BarID +0x60, QuestStartScreened +0x64, GotSuperScreamer +0x65.
// Shape (landed 0x00E1A870 form): one zero register for all member stores (the float zero is an integer store), scheduled
// into the first string's argument pushes; both literals are implicit conversion temporaries built right to left.
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
class CQ_HeroSoulsMotherScript {
public:
    unsigned char         _pad_0x00[0x40];
    CGameScriptInterface* Game;                       // +0x40
    unsigned char         _pad_0x44[4];
    bool                  PlayedMotherCS;             // +0x48
    unsigned char         _pad_0x49[0x58 - 0x49];     // csargs +0x4c
    long                  ScreamersAlive;             // +0x58
    float                 BarToFill;                  // +0x5c
    long                  BarID;                      // +0x60
    bool                  QuestStartScreened;         // +0x64
    bool                  GotSuperScreamer;           // +0x65
    void Init();
};
} // namespace NScript
using NScript::CQ_HeroSoulsMotherScript;
void NScript::CQ_HeroSoulsMotherScript::Init()
{
    PlayedMotherCS = false;
    BarToFill = 0;
    BarID = 0;
    ScreamersAlive = 0;
    QuestStartScreened = false;
    GotSuperScreamer = false;
    Game->AddQuestRegion("Q_HeroSoulsMother", "OakvaleMemorialGarden");
}