// CV_FisticuffsClubScript::KeepVariablesUpToDate() -- script thread: outside the fisticuffs club the hero may use will and weapons, teleport, change morality, is killable and guards punish crimes (confiscated items come back); while the hero is inside the club all of that is switched the other way, until the hero leaves again
// Leaf: KeepVariablesUpToDate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E824E0, length 367 bytes (+ int3 padding), plain `ret`.
// Identity: CV_FisticuffsClubScript::Main (0x00E82220) spawns this thread as "KeepVariablesUpToDate" (string 0x012EAFD4,
// CSpawnedFunc record at 0x00E82409); the manifest's "CQ_HangingTreeEvilScript::BanditYells" label is propagated.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (landed); through the game script interface (+0x40, vtable
// 0x01260F0C): NewScriptFrame (slot 7), GetHero (slot 70), SetHeroWillAsUsable (131), SetHeroWeaponsAsUsable (132),
// SetGuardsIgnoreCrimes (134), ReturnAllConfiscatedItemsToHero (140), EntitySetAsKillable (517),
// SetTeleportingAsActive (565), SetMoralityChangingAsEnabled (573) -- slot names from the vtable's manifest labels.
// Layout: retail CV_FisticuffsClubScript = Ego_r.pdb - 0x14 (as the landed SetFightCount 0x00E82800):
// HeroInFisticuffsClub +0x53 (PDB +0x67); the script data block (+0x44) mirrors the flag at +0xE3 for other scripts.
// Shape: the landed BanditYells (0x00D69CD0) thread loop: a rotated `while (!terminating)` with each wait loop
// returning on termination and a second termination test after it.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CScriptThing;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CFisticuffsScriptData {                              // the script data block (+0x44)
    unsigned char _pad00[0xe3];
    bool          HeroInFisticuffsClub;                     // +0xe3
};

namespace NScript {
class CV_FisticuffsClubScript : public CScriptBase {
public:
    CGameScriptInterface*  GSI;                             // +0x40
    CFisticuffsScriptData* Data;                            // +0x44
    unsigned char          _pad48[0x53 - 0x48];
    bool                   HeroInFisticuffsClub;            // +0x53 (PDB +0x67)
    void KeepVariablesUpToDate();
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

void NScript::CV_FisticuffsClubScript::KeepVariablesUpToDate()
{
    while (!IsActiveThreadTerminating()) {
        Data->HeroInFisticuffsClub = false;
        GSI->SetHeroWillAsUsable(true);
        GSI->SetHeroWeaponsAsUsable(true);
        GSI->SetTeleportingAsActive(true);
        GSI->ReturnAllConfiscatedItemsToHero();
        GSI->EntitySetAsKillable(GSI->GetHero(), true, true);
        GSI->SetMoralityChangingAsEnabled(true);
        GSI->SetGuardsIgnoreCrimes(false);
        while (!HeroInFisticuffsClub) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Data->HeroInFisticuffsClub = true;
        GSI->SetTeleportingAsActive(false);
        GSI->EntitySetAsKillable(GSI->GetHero(), false, true);
        GSI->SetMoralityChangingAsEnabled(false);
        GSI->SetGuardsIgnoreCrimes(true);
        GSI->SetHeroWillAsUsable(false);
        GSI->SetHeroWeaponsAsUsable(false);
        while (HeroInFisticuffsClub) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->NewScriptFrame();
    }
}