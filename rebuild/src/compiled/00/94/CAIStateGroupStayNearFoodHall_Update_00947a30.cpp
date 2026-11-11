// CAIStateGroup_StayNearFoodHall::Update() -- repeatedly walk to the food hall and wait while cooking
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: 0x00947a30, length 244. Uses the generated CAIStateGroupBase header for the +0x14 base end;
// local derived members model Position (+0x14) and its intelligent pointer (+0x20), as retail accesses them there.
// Callees: GetPItem @ 0x00A01B50, IsInWorkHours @ 0x00947840, MoveToPosProximity @ 0x0097E890,
// LowerBound @ 0x004365B0, GFGetVillage @ 0x007EE540, IsCookTime @ 0x00658BB9, Terminate @ 0x008FCF40.
// The loop continues only while the move action reports 4 and the hall remains live and cooking time continues.
// Name fix (agentNF7 2026-11-10): the local CBaseIntelligentPointer stand-in replaced by the shared engine/CIntelligentPointer.h (Ego_r.pdb CBaseIntelligentPointer: GetPItem returns CBaseObject*, SetPItem takes CBaseObject*, virtual dtor), so the calls bind the PDB spellings.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CThing;
#include "engine/C3DVector.h"
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*) const; };
class CThing { public: unsigned char pad0[0x20]; unsigned long flags20; unsigned long flags24; unsigned char pad28[0x1c]; CInterfaceMap interfaces; unsigned char pad4c[0x45]; unsigned char dead; };
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CBaseIntelligentPointer 0x00A01B10 / ~ 0x00A01C10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (Ego_r.pdb)
struct CTCVillager;
class CTCVillage { public: bool IsCookTime_00658bb9(); };
struct CAIStateGroup_StayNearFoodHall : CAIStateGroupBase {
    C3DVector Position;
    CBaseIntelligentPointer PFoodHall;
    bool IsInWorkHours();
    void Update();
    long AIAction_MoveToPosProximity(long, C3DVector, float, bool*, long, bool, bool, bool, CThing*, bool, float);
    void Terminate();
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)
void CAIStateGroup_StayNearFoodHall::Update()
{
    CThing* foodHall;
    CThing* creature;
    CInterfaceMap::Entry* it;
    CInterfaceMap* map;
    CTCVillage* village;
    bool reached;
    long result;
    long key;
    if (!reinterpret_cast<CThing*>(PFoodHall.GetPItem())) goto done;
    foodHall = reinterpret_cast<CThing*>(PFoodHall.GetPItem());
    if (foodHall->dead & 1) goto done;
    if (!IsInWorkHours()) goto done;
    do {
        result = AIAction_MoveToPosProximity(2, Position, 4.0f, &reached, 3, true, false, true, 0, true, 0.0f);
        if (!reinterpret_cast<CThing*>(PFoodHall.GetPItem())) goto done;
        foodHall = reinterpret_cast<CThing*>(PFoodHall.GetPItem());
        if (foodHall->dead & 1) goto done;
        creature = (CThing*)PBrain->Creature;
        if (!(creature->flags24 & 8)) goto done;
        key = 0x23;
        map = &creature->interfaces;
        it = map->LowerBound(&key);
        if (it == map->last || it->type > 0x23) it = map->last;
        village = ((CTCVillageMember*)((CTCVillager*)it->component))->GetPVillage();
        if (!village || !village->IsCookTime_00658bb9()) goto done;
    } while (result == 4);
done:
    Terminate();
}