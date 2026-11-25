// CAIStateGroup_StayNearFoodHall::Update() -- repeatedly walk to the food hall and wait while cooking
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: 0x00947a30, length 244. Uses the generated CAIStateGroupBase header for the +0x14 base end;
// local derived members model Position (+0x14) and its intelligent pointer (+0x20), as retail accesses them there.
// Callees: GetPItem @ 0x00A01B50, IsInWorkHours @ 0x00947840, MoveToPosProximity @ 0x0097E890,
// LowerBound @ 0x004365B0, GFGetVillage @ 0x007EE540, IsCookTime @ 0x00658BB9, Terminate @ 0x008FCF40.
// The loop continues only while the move action reports 4 and the hall remains live and cooking time continues.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
// NF51 (round 7): the AI state-group base is the retail branch of the shared engine/CAIStateGroupBase.h (was its flattened default
// plus a local AIAction_MoveToPosProximity / Terminate on the derived class): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h, and
// Terminate 0x008FCF40 the base's ?Terminate@CAIStateGroupBase@@QAEXXZ; Update overrides the base's slot 12
// (?Update@CAIStateGroup_StayNearFoodHall@@UAEXXZ, Ego_r.pdb / FableWin.pdb).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CThing;
#include "engine/C3DVector.h"
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in CInterfaceMap (?LowerBound@CInterfaceMap@@QBEPAUEntry@1@PBJ@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE spelling at 0x004365B0.
class CTCBase;
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public: unsigned char pad0[0x20]; unsigned long flags20; unsigned long flags24; unsigned char pad28[0x1c]; CInterfaceMap interfaces; unsigned char pad54[0x3d]; unsigned char dead; };
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
struct CTCVillager;
class CTCVillage { public: bool IsEatTime() const; };   // 0x00658BB9 ?IsEatTime@CTCVillage@@QBE_NXZ (Ego_r + FableWin, the self row; NF285, was IsCookTime_00658bb9)
struct CAIStateGroup_StayNearFoodHall : CAIStateGroupBase {
public:
    C3DVector Position;
    CBaseIntelligentPointer PFoodHall;
    bool IsInWorkHours();
    void Update();
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)
void CAIStateGroup_StayNearFoodHall::Update()
{
    CThing* foodHall;
    CThing* creature;
    const CInterfaceMap::value_type* it;
    const CInterfaceMap* map;
    CTCVillage* village;
    bool reached;
    long result;
    ETCInterfaceType key;
    if (!reinterpret_cast<CThing*>(PFoodHall.GetPItem())) goto done;
    foodHall = reinterpret_cast<CThing*>(PFoodHall.GetPItem());
    if (foodHall->dead & 1) goto done;
    if (!IsInWorkHours()) goto done;
    do {
        result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, Position, 4.0f, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
        if (!reinterpret_cast<CThing*>(PFoodHall.GetPItem())) goto done;
        foodHall = reinterpret_cast<CThing*>(PFoodHall.GetPItem());
        if (foodHall->dead & 1) goto done;
        creature = (CThing*)PBrain->Creature;
        if (!(creature->flags24 & 8)) goto done;
        key = (ETCInterfaceType)0x23;
        map = &creature->interfaces;
        it = map->LowerBound(key);
        if (it == map->end() || it->first > 0x23) it = map->end();
        village = ((CTCVillageMember*)((CTCVillager*)it->second))->GetPVillage();
        if (!village || !village->IsEatTime()) goto done;
    } while (result == 4);
done:
    Terminate();
}