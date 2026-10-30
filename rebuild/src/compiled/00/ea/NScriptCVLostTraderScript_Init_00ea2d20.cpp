// CV_LostTraderScript::Init() -- registers the quest's two regions (BanditCampPath1, DemonDoor_BanditCampPath), clears the five flags and sets DistanceForBrotherToReunite to 10
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA2D20, 177 bytes (padded to 192), plain `ret`.
// Identity: slot 3 of vtable 0x012EDCE8 (stored by the ctor at 0x00EA2CF0); "V_LostTrader" and the stores match Ego_r.pdb
//   CV_LostTraderScript - 0x14: g_bLostTraderKilled..ReachedTrader +0x48..+0x4c, DistanceForBrotherToReunite +0x50.
// Callees: CGameScriptInterface slot 271 AddQuestRegion(const CCharString&, const CCharString&) (+0x43c, 0x00891820),
//   CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0.
//   Strings: "BanditCampPath1" 0x012C83AC, "V_LostTrader" 0x012C4F98, "DemonDoor_BanditCampPath" 0x012C836C.
// Shape: implicit conversion temporaries (right argument first), as the CQ_SummoningTheShipScript::Init twin 0x00DF1230;
//   the bool zeros share `xor al,al`.
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
class CV_LostTraderScript {
public:
    unsigned char _base[0x40];                     // CScriptBase
    CGameScriptInterface* Game;                    // +0x40
    void* Data;                                    // +0x44
    bool g_bLostTraderKilled;                      // +0x48
    bool g_bTraderBrotherKilled;                   // +0x49
    bool g_bQuestFailed;                           // +0x4a
    bool g_bTraderHit;                             // +0x4b
    bool ReachedTrader;                            // +0x4c
    long DistanceForBrotherToReunite;              // +0x50
    long ReasonForFailure;                         // +0x54
    void Init();
};
} // namespace NScript
using NScript::CV_LostTraderScript;
void NScript::CV_LostTraderScript::Init()
{
    Game->AddQuestRegion("V_LostTrader", "BanditCampPath1");
    Game->AddQuestRegion("V_LostTrader", "DemonDoor_BanditCampPath");
    g_bLostTraderKilled = false;
    g_bTraderBrotherKilled = false;
    g_bQuestFailed = false;
    g_bTraderHit = false;
    ReachedTrader = false;
    DistanceForBrotherToReunite = 10;
}