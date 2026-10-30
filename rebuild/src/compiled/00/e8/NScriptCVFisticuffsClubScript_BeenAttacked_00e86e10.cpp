// CV_FisticuffsClubScript::BeenAttacked() -- outside the club, count one more attack warning, or once the def's warning limit is reached make everybody attack
// Leaf: ?BeenAttacked@CV_FisticuffsClubScript@NScript@@
// Retail: Fable.exe @ 0x00E86E10, 78 bytes (+ int3 padding). Called from three places (0x00E852E1, 0x00E85F7C, 0x00E88354).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (the script's terminate check before each action).
// Layout: Ego_r.pdb CV_FisticuffsClubScript - 0x14: HeroInFisticuffsClub +0x53, AttackWarnings +0x84, EverybodyAttack
// +0x8c; the warning limit is the long at +0x624 of the script def @ 0x0143E90C (descriptive name).
// Shape: nested ifs, each action preceded by `if (terminating) return;`; `++AttackWarnings` is `inc dword ptr [esi+84h]`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04..+0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

class CScriptDef { public:
    unsigned char _pad[0x624];
    long FisticuffsAttackWarningLimit;                   // +0x624
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

namespace NScript {
class CV_FisticuffsClubScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x53 - 0x48];
    bool HeroInFisticuffsClub;                           // +0x53
    unsigned char _pad54[0x84 - 0x54];
    long AttackWarnings;                                 // +0x84
    long FightCount;                                     // +0x88
    bool EverybodyAttack;                                // +0x8c
    void BeenAttacked();
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;

void NScript::CV_FisticuffsClubScript::BeenAttacked()
{
    if (!HeroInFisticuffsClub) {
        if (IsActiveThreadTerminating())
            return;
        if (AttackWarnings < pGlobalScriptDef->FisticuffsAttackWarningLimit) {
            if (IsActiveThreadTerminating())
                return;
            ++AttackWarnings;
        } else {
            if (IsActiveThreadTerminating())
                return;
            EverybodyAttack = true;
        }
    }
}