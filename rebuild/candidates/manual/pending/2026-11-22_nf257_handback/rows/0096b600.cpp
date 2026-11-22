// CAIStateGroup_WhisperRetreat::Update() -- unsheathe when unarmed; while the target lives: flag its stealth component as in danger from the creature, jump to safety when the target is within 3 units, then advance from boss phase 0 when ready or, in phase 2, use will and terminate; terminate
// Name fix (NF96 part 2, q11460): the local CTCStealth declared SetAsInDanger (CThing* source, long status) and bound a stand-in spelling of 0x00747B80;
//   now the Ego_r.pdb + FableWin.pdb ?SetAsInDanger@CTCStealth@@QAEXABVCThing@@W4EDangerStatus@NStealth@@@Z (engine/CTCStealth.h signature;
//   the reference and the enum push the same words).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096B600, length 355 bytes (plain `ret`, virtual).
// Callees: CTCCarrying::IsThingCarryingWeapon(const CThing&) @ 0x006A1540 (static __fastcall), CThingCreatureBase::UnsheatheMeleeWeapon
//          @ 0x006649C0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 173 = bit 13 of +0x34 = CTCBoss, 54 = bit 22
//          of +0x24 = CTCStealth), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCStealth::SetAsInDanger(const CThing&, NStealth::EDangerStatus)
//          @ 0x00747B80, CThing::GetPosition @ 0x004C73D0, CAIStateGroup_WhisperRetreat::JumpToSafety @ 0x0096AC20 and
//          UseWill @ 0x0096AE50, CTCBoss::GetBossPhase @ 0x007AED10, IsReadyForPhaseChange @ 0x007AEE90, SetNextBossPhase
//          @ 0x007AEDA0, CAIStateGroupBase::Terminate @ 0x008FCF40; float pool 3.0f @ 0x0122DED4.
// Shape: the boss lookup is the out-parameter CThing::GetTC<CTCBoss> (miss path reads the never-written slot); IS_THING_ALIVE is the
//          two-GetPItem inline test and the stealth GetTC is a third GetPItem whose hit path alone calls SetAsInDanger; the
//          planar distance (`dx*dx + dy*dy`, dy squared first) is compared `< 3.0f` (`fcomp ; test ah,5 ; jp`); the phase
//          switch is `sub eax,0 ; je` / `sub eax,2 ; jne` with the phase-2 arm ending in its own Terminate + epilogue.
// Layout: engine/CAIStateGroup_WhisperRetreat.h (FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT, size 0x30) over engine/CAIStateGroup_EnemyCreatureBase.h:
//          the target is the inherited PTargetThing (CIntelligentPointer<CThingCreatureBase>, +0x14); PArenaCentre +0x1c is unused here.
// Leaf: ?Update@CAIStateGroup_WhisperRetreat@@UAEXXZ
// Names (agentNF257, 2026-11-22, q12724): was on row-local CThing / CIntelligentPointer / CAIStateGroupBase / CAIBrain / CTCBoss /
//   CTCStealth / CTCCarrying / CAIStateGroup_WhisperRetreat models, binding ?UnsheatheMeleeWeapon@CThing@@QAEXXZ (0x006649C0) and the public
//   ?JumpToSafety@ / ?UseWill@CAIStateGroup_WhisperRetreat@@QAEXXZ (0x0096AC20 / 0x0096AE50). Now on the shared headers: the callees take
//   the PDB spellings ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ (Ego_r / FableWin / Ego_d; engine/CThingCreatureBase.h
//   UNSHEATHEMELEEWEAPON, the spelling 0x00664A50 / 0x00666830 / 0x008B7BA0 / 0x00890C80 bind) and the protected IAE pair (Ego_r + FableWin;
//   the landed UseWill row 0x0096AE50 defines IAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCBOSS_ISREADYFORPHASECHANGE
#define FABLE_RETAIL_NSTEALTH_EDANGERSTATUS
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT_UPDATE
#include "fable_string.h"
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCBoss.h"
#include "engine/CTCStealth.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_WhisperRetreat.h"

void CAIStateGroup_WhisperRetreat::Update()
{
    if (!CTCCarrying::IsThingCarryingWeapon(GetCreature()))
        GetCreature().UnsheatheMeleeWeapon();
    CTCBoss* boss;
    GetCreature().GetTC(&boss);
    if (IS_THING_ALIVE(PTargetThing)) {
        CTCStealth* stealth;
        if (PTargetThing->GetTC(&stealth))
            stealth->SetAsInDanger(GetCreature(), NStealth::DANGER_STATUS_COMBAT);
        CThingAICreature& creature = GetCreature();
        CThingCreatureBase* target = PTargetThing.Get();
        const C3DVector& pos = creature.GetPos();
        const C3DVector& target_pos = target->GetPos();
        float dx = target_pos.X - pos.X;
        float dy = target_pos.Y - pos.Y;
        if ((float)sqrt(dx * dx + dy * dy) < 3.0f)
            JumpToSafety();
        switch (boss->GetBossPhase()) {
        case 0:
            if (boss->IsReadyForPhaseChange())
                boss->SetNextBossPhase(1, false);
            break;
        case 2:
            UseWill();
            Terminate();
            return;
        default:
            break;
        }
    }
    Terminate();
}
