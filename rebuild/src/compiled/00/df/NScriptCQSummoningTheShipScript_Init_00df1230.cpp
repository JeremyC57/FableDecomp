// CQ_SummoningTheShipScript::Init() -- registers the quest's two regions (HookCoast, LostBay) and resets the summoner count, attack wave 1 and the three flags
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF1230, 174 bytes (padded to 176), plain `ret`.
// Identity: the manifest label "CV_LostTraderScript::Init" is propagated and wrong: this body is slot 3 of vtable 0x012DE10C,
//   stored by the ctor at 0x00DF11F0 (CommentaryTimer at +0x54), and its strings name "Q_SummoningTheShip"; the stores match
//   Ego_r.pdb CQ_SummoningTheShipScript - 0x14: SummonersAlive +0x48, CurrentAttackWave +0x4c, LighthouseStarted +0x50,
//   SummonerAttacksStarted +0x51, MissionFailed +0x52.
// Callees: CGameScriptInterface slot 271 AddQuestRegion(const CCharString&, const CCharString&) (+0x43c, 0x00891820),
//   CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0.
//   Strings: "HookCoast" 0x012CBB4C, "Q_SummoningTheShip" 0x012C47E4, "LostBay" 0x012C4744.
// Shape: both literals are implicit conversion temporaries (right argument built first); integer zero shared in eax.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_SummoningTheShipScript {
public:
    unsigned char _base[0x40];                     // CScriptBase
    CGameScriptInterface* Game;                    // +0x40
    void* Data;                                    // +0x44
    long SummonersAlive;                           // +0x48
    long CurrentAttackWave;                        // +0x4c
    bool LighthouseStarted;                        // +0x50
    bool SummonerAttacksStarted;                   // +0x51
    bool MissionFailed;                            // +0x52
    void Init();
};
} // namespace NScript
using NScript::CQ_SummoningTheShipScript;
void NScript::CQ_SummoningTheShipScript::Init()
{
    Game->AddQuestRegion("Q_SummoningTheShip", "HookCoast");
    Game->AddQuestRegion("Q_SummoningTheShip", "LostBay");
    SummonersAlive = 0;
    CurrentAttackWave = 1;
    LighthouseStarted = false;
    SummonerAttacksStarted = false;
    MissionFailed = false;
}