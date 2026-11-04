// CAIStateGroup_CreatureRangedCombat2::IsAbleToFire() const -- a perceiving creature with a live target may fire, unless it is a guard village member whose village holds the hero a criminal and the target's fine dialogue is allowed (then it should talk, not shoot)
// Leaf: ?IsAbleToFire@CAIStateGroup_CreatureRangedCombat2@@ABE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091EA70, length 240 bytes. Identity: Ego_r.pdb
//   ?IsAbleToFire@CAIStateGroup_CreatureRangedCombat2@@ABE_NXZ (private const).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE on PTargetThing +0x14, then the target),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (GetTC<CTCVillageMember> 0x23, GetTC<CTCFineDialogue> 0xF3),
//   CTCVillageMember::GetPVillage() const @ 0x007EE540 (twice), CTCVillage::IsHeroCriminal @ 0x0065BFCE,
//   CTCFineDialogue::IsAllowed @ 0x007E2790.
// Shape: HasTC(TCI_PERCEIVE_THING 0x34) and IS_THING_ALIVE, then HasTC(TCI_GUARD 0xB4) and GetTC<CTCVillageMember>; the member
//   is asked for its village twice; every inner failure falls to `mov al,1`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CTCVILLAGE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#define CAIStateGroup_CreatureRangedCombat2 DonorCAIStateGroup_CreatureRangedCombat2
#include "engine/CAIStateGroup_CreatureRangedCombat2.h"
#undef CAIStateGroup_CreatureRangedCombat2

class CTCFineDialogue : public CTCBase {                            // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_FINE_DIALOGUE; }
    bool IsAllowed();                                               // 0x007E2790
};
template<class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { return p.Get() != 0 && !p.Get()->IsDead(); }

class CAIStateGroup_CreatureRangedCombat2 : public CAIStateGroupBase {
private:
    bool IsAbleToFire() const;
protected:
    CIntelligentPointer<CThingCreatureBase> PTargetThing;           // +0x14
};

bool CAIStateGroup_CreatureRangedCombat2::IsAbleToFire() const
{
    if (PBrain->Creature->HasTC(TCI_PERCEIVE_THING) && IS_THING_ALIVE(PTargetThing)) {
        CThingAICreature* creature = PBrain->Creature;
        CTCVillageMember* member;
        if (creature->HasTC(TCI_GUARD) && creature->GetTC(&member)) {
            if (member->GetPVillage() && member->GetPVillage()->IsHeroCriminal()) {
                CTCFineDialogue* fine;
                if (PTargetThing->GetTC(&fine) && fine->IsAllowed())
                    return false;
            }
        }
        return true;
    }
    return false;
}