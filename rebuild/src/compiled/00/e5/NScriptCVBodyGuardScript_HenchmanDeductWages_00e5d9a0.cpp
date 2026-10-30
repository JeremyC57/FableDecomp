// CV_BodyGuardScript::HenchmanDeductWages() -- script thread: while henchmen are hired and the bodyguards are neither
// told to wait nor in limbo, charge the hero the total wages each pay period; when he cannot pay, flag it until the
// henchmen leave; whenever the list is empty re-arm the freaked-out timer for 180; one script frame per pass
// Leaf: ?HenchmanDeductWages@CV_BodyGuardScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5D9A0, 385 bytes (padded to 400).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; CGameScriptInterface slots 7 NewScriptFrame (+0x1c),
//          89 SetTimer (+0x164, through g_pGameScriptInterface 0x0143E8F8), 126 GiveHeroGold (+0x1f8), 127 GetHeroGold
//          (+0x1fc), 376 Pause (+0x5e0).
// Layout (retail = Ego_r.pdb CV_BodyGuardScript - 0x14, as the landed CheckForHenchmanFiredOverride 0x00E5D900): Game
//          +0x40, Data +0x44 (CQ_SunnyvaleMasterData: HenchmanOverrideState +0xc, BodyGuardsMustStandAndWait +0xe3,
//          BodyGuardsInLimbo +0xe4), TotalWages +0x48, HeroCannotAffordHelp +0x4c, timers +0x54.., HenchmanList +0x64;
//          script def (0x0143E90C) HenchmanPayTimeDelayInSeconds +0x4a4.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_SunnyvaleMasterData {                    // Ego_r.pdb offsets (no debug-only members before +0xe5)
    unsigned char _pad00[0x0c];
    long          HenchmanOverrideState;           // +0x0c
    unsigned char _pad10[0xe3 - 0x10];
    bool          BodyGuardsMustStandAndWait;      // +0xe3
    bool          BodyGuardsInLimbo;               // +0xe4
};
class CScriptDef { public:
    unsigned char _pad[0x4a4];
    long          HenchmanPayTimeDelayInSeconds;   // +0x4a4 (Ego_r.pdb +0x4d8)
};
extern CScriptDef*           pGlobalScriptDef;             // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                    // a script timer: the interface's timer index
    long Index;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
class CScriptThing { public:                              // 12 bytes (vtable 0x01238C8C)
    void* __vftable;
    long  m_data[2];
};
struct CScriptThingVector {                        // STLport vector<CScriptThing>
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_BodyGuardScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                  // +0x40
    CQ_SunnyvaleMasterData* Data;                  // +0x44
    long   TotalWages;                             // +0x48
    bool   HeroCannotAffordHelp;                   // +0x4c
    bool   HeroWieldingWeapon;                     // +0x4d
    long   HeroStoodStillCounter;                  // +0x50 (CCounter)
    CTimer HenchmanLastIdleChatTimer;              // +0x54
    CTimer HenchmanLastBoredChatTimer;             // +0x58
    CTimer CommentaryTimer;                        // +0x5c
    CTimer HenchmanFreakedOutThePlayerTimer;       // +0x60
    CScriptThingVector HenchmanList;               // +0x64
    void HenchmanDeductWages();
};
} // namespace NScript
using NScript::CV_BodyGuardScript;
void NScript::CV_BodyGuardScript::HenchmanDeductWages()
{
    while (!IsActiveThreadTerminating())
    {
        if (TotalWages > 0 && !Data->BodyGuardsMustStandAndWait && !Data->BodyGuardsInLimbo)
        {
            if (IsActiveThreadTerminating())
                return;
            if (Game->GetHeroGold() >= TotalWages)
            {
                if (IsActiveThreadTerminating())
                    return;
                if (Data->HenchmanOverrideState != 1)
                {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->GiveHeroGold(-TotalWages);
                    Game->Pause((float)pGlobalScriptDef->HenchmanPayTimeDelayInSeconds);
                }
            }
            else
            {
                if (IsActiveThreadTerminating())
                    return;
                HeroCannotAffordHelp = true;
                while (HenchmanList.size() != 0)
                {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                HeroCannotAffordHelp = false;
            }
        }
        if (HenchmanList.size() == 0)
        {
            if (IsActiveThreadTerminating())
                return;
            HenchmanFreakedOutThePlayerTimer.SetTime(180);
        }
        Game->NewScriptFrame();
    }
}