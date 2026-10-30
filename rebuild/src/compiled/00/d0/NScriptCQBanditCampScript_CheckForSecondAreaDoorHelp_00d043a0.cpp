// Leaf: CheckForSecondAreaDoorHelp
// Grade: RELOCATION_MATCH 948/948 on the corrected span 0x00D043A0+948 (code + embedded jump table; bounds audit) -- catalog row 976 B over-captured 0x00D04760; really CQ_BanditCampScript::CheckForSecondAreaDoorHelp (catalog: EndMission, propagated)
// CQ_BanditCampScript::CheckForSecondAreaDoorHelp() -- once the bandit raid is under way, while the hero lingers outside BanditCampResidential the guildmaster cycles through his THROUGH_GATE hints every 20 timer ticks; stops when the hero reaches BanditCampBoss
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D043A0, length 964 bytes, plain `ret` (+ a five-entry switch table).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CTimer (inline ctor/dtor on the global interface 0x0143E8F8, slots 87..90; out-of-line ~CTimer 0x00CD4470
//   at the return before the switch); CGameScriptInterface slots 7 NewScriptFrame, 11 IsLevelLoaded, 274
//   HeroReceiveMessageFromGuildMaster. Strings "BanditCampResidential", "BanditCampBoss",
//   "TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10" .. "_50".
// Layout: Ego_r.pdb CQ_BanditCampScript minus 0x14 (retail CScriptBase): ActivatedBanditRaid +0x4d.
// Identity: spawned as "CheckForSecondAreaDoorHelp" (string 0x012C8910) by the CQ_BanditCampScript spawn list at
//   0x00D041B9 (member pointer stored at 0x00D041E3); the catalog's CV_ChapelOfEvilScript::EndMission is a propagated label.
// Shape: generated script code; `for (;;)` with the terminate return first; the hint switch's last case loops back to
//   stage 3 (cases 2 and 4 share the store).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;                   // 0x0143E8F8
class CTimer { public:                                             // a script timer: the interface's timer index
    long Index;
    CTimer() { Index = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }   // out of line 0x00CD4470
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_BanditCampScript : public CScriptBase {
public:
    CGameScriptInterface*    Game;                                     // +0x40
    void*         Data;                                     // +0x44
    unsigned char _pad48[0x4d - 0x48];
    bool          ActivatedBanditRaid;                      // +0x4d
    void CheckForSecondAreaDoorHelp();
};
} // namespace NScript
using NScript::CQ_BanditCampScript;

void NScript::CQ_BanditCampScript::CheckForSecondAreaDoorHelp()
{
    while (!ActivatedBanditRaid) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    timer.SetTime(5);
    long stage = 0;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("BanditCampResidential")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsLevelLoaded("BanditCampBoss")) {
                if (IsActiveThreadTerminating())
                    return;
                return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() <= 0) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            switch (stage) {
            case 0:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10", "", true, true);
                stage = 1;
                break;
            case 1:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_20", "", true, true);
                stage = 2;
                break;
            case 2:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_30", "", true, true);
                stage = 3;
                break;
            case 3:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_40", "", true, true);
                stage = 4;
                break;
            case 4:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_50", "", true, true);
                stage = 3;
                break;
            }
            timer.SetTime(20);
        }
        Game->NewScriptFrame();
    }
}