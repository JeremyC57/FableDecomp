// CTCCreatureGenerator::UpdateUndead() -- for every triggerer and every created undead: outside the trigger radius a
// surfaced undead returns to the earth and a buried one (when free to act) is moved to a fresh generation point near the
// triggerer and rises with a strike; inside the radius a buried undead rises with a strike
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
//
// Retail 0x0074ACE0, 624 bytes, plain `ret` (0x498-byte frame: four 0x114-byte action temporaries).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos @ 0x004C73D0, NCreatureMode::IsUnderground
// @ 0x0085F120 (__fastcall), CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase*, CThing*) @ 0x0074DAC0,
// CCharString @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCombatActionBase::SetCombatAnimation @ 0x008574D0,
// CThingCreatureBase::IsFreeToPerformAction @ 0x00662210 / SetCurrentAction @ 0x006644F0, ~CCreatureActionBase @ 0x0062E900,
// GetUndeadGenerationPoint @ 0x0074A260, physics slot 31 (+0x7c, set position), CCreatureAction_UndeadReturnToEarth
// @ 0x008EC6A0. RiseUpStrike vtable 0x01268F44, name "RISE_UP_STRIKE" @ 0x01268F30.
// Layout (retail, not the PDB header's): CreatedCreatures list head +0x18, CreatureTriggerers start/finish +0x1c/+0x20,
// trigger radius +0x2c, UndeadTriggered +0x88.
// Port (NF98, round 8, q11261 / q11262): the namespace-N_0062e900 CCreatureActionBase stand-in and the local CThing / CThingCreatureBase /
//   C2DVector / CCombatActionBase / CCreatureAction_RiseUp / RiseUpStrike / CCreatureAction_UndeadReturnToEarth / CTCPhysicsBase / list-node /
//   CTCCreatureGenerator models replaced by the shared headers (the CreateCreaturesUndead 0x0074AF50 recipe): SetCurrentAction 0x006644F0
//   and ~CCombatActionBase 0x0062E900 bind the PDB spellings. The strike temporary is CCreatureAction_UndeadRiseUp (vtable 0x01268F44 RTTI;
//   FableWin.pdb inline ctor (creature, target, bool), NF98 _CLASS opt-in), was a CCreatureAction_RiseUpStrike stand-in; the RiseUp ctor
//   0x0074DAC0 is ??0CCreatureAction_RiseUp@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z; UndeadReturnToEarth takes the Ego_r.pdb
//   (CThingCreatureBase&) ctor (NF98 _CLASS opt-in); IsUnderground is ?IsUnderground@NCreatureMode@@YI_NABVCThing@@@Z; GetUndeadGenerationPoint
//   takes (const CThingCreatureBase&, C3DVector&); own leaf the protected Ego_r.pdb / FableWin.pdb ?UpdateUndead@CTCCreatureGenerator@@IAEXXZ
//   (NF98 opt-in FABLE_RETAIL_CTCCREATUREGENERATOR_TRIGGERERS, which also types CreatureTriggerers +0x1C). The physics call at +0x7C is
//   slot 31 ResetPosition (the shared CTCPhysicsBase order; slot 30 +0x78 is SetPosition), was a local slot-31 "SetPosition".
// Leaf: ?UpdateUndead@CTCCreatureGenerator@@IAEXXZ
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRISEUP_CLASS
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRETURNTOEARTH_CLASS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCCREATUREGENERATOR_UNDEAD
#define FABLE_RETAIL_CTCCREATUREGENERATOR_TRIGGERERS
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CDefPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NCreatureMode.h"
#include "engine/CArray.h"
#include "engine/CDefIndex.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_RiseUp.h"
#include "engine/CCreatureAction_UndeadRiseUp.h"
#include "engine/CCreatureAction_UndeadReturnToEarth.h"
#include "engine/CTCCreatureGenerator.h"

void CTCCreatureGenerator::UpdateUndead()
{
    if (CreatureTriggerers.empty())
        UndeadTriggered = false;
    for (std::vector<CIntelligentPointer<CThingCreatureBase> >::iterator it = CreatureTriggerers.begin(); it != CreatureTriggerers.end(); ++it) {
        CThingCreatureBase* triggerer = it->Get();
        for (std::list<CIntelligentPointer<CThing> >::iterator node = CreatedCreatures.begin(); node != CreatedCreatures.end(); ++node) {
            CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(node->Get());
            const C3DVector& creaturePos = creature->GetPos();
            const C3DVector& triggererPos = triggerer->GetPos();
            C3DVector delta;
            delta.X = triggererPos.X - creaturePos.X;
            delta.Y = triggererPos.Y - creaturePos.Y;
            bool underground = NCreatureMode::IsUnderground(*creature);
            if (delta.Y * delta.Y + delta.X * delta.X > GenParams.SelfTriggerRadius * GenParams.SelfTriggerRadius) {
                if (underground) {
                    bool free;
                    {
                        CCreatureAction_UndeadRiseUp action(*creature, *triggerer, true);
                        free = creature->IsFreeToPerformAction(action);
                    }
                    C3DVector pos;
                    if (free && GetUndeadGenerationPoint(*triggerer, pos)) {
                        creature->GetPhysicsTC()->ResetPosition(pos);
                        creature->SetCurrentAction(CCreatureAction_UndeadRiseUp(*creature, *triggerer, true));
                    }
                } else {
                    creature->SetCurrentAction(CCreatureAction_UndeadReturnToEarth(*creature));
                }
            } else if (underground) {
                creature->SetCurrentAction(CCreatureAction_UndeadRiseUp(*creature, *triggerer, true));
            }
        }
    }
}
