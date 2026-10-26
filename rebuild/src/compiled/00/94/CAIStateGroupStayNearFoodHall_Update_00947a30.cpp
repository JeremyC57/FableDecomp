// CAIStateGroup_StayNearFoodHall::Update() -- repeatedly walk to the food hall and wait while cooking
// Retail: 0x00947a30, length 244. Uses the generated CAIStateGroupBase header for the +0x14 base end;
// local derived members model Position (+0x14) and its intelligent pointer (+0x20), as retail accesses them there.
// Callees: GetPItem @ 0x00A01B50, IsInWorkHours @ 0x00947840, MoveToPosProximity @ 0x0097E890,
// LowerBound @ 0x004365B0, GFGetVillage @ 0x007EE540, IsCookTime @ 0x00658BB9, Terminate @ 0x008FCF40.
// The loop continues only while the move action reports 4 and the hall remains live and cooking time continues.
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CThing;
class C3DVector { public: float X,Y,Z; C3DVector() : X(0),Y(0),Z(0) {} C3DVector(const C3DVector& v) : X(v.X),Y(v.Y),Z(v.Z) {} };
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*) const; };
class CThing { public: unsigned char pad0[0x20]; unsigned long flags20; unsigned long flags24; unsigned char pad28[0x1c]; CInterfaceMap interfaces; unsigned char pad4c[0x45]; unsigned char dead; };
struct CBaseIntelligentPointer { virtual void v00(); void* Handle; CThing* GetPItem(); };
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
CTCVillage* __fastcall GFGetVillage(CTCVillager*); // 0x007ee540
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
    if (!PFoodHall.GetPItem()) goto done;
    foodHall = PFoodHall.GetPItem();
    if (foodHall->dead & 1) goto done;
    if (!IsInWorkHours()) goto done;
    do {
        result = AIAction_MoveToPosProximity(2, Position, 4.0f, &reached, 3, true, false, true, 0, true, 0.0f);
        if (!PFoodHall.GetPItem()) goto done;
        foodHall = PFoodHall.GetPItem();
        if (foodHall->dead & 1) goto done;
        creature = (CThing*)PBrain->Creature;
        if (!(creature->flags24 & 8)) goto done;
        key = 0x23;
        map = &creature->interfaces;
        it = map->LowerBound(&key);
        if (it == map->last || it->type > 0x23) it = map->last;
        village = GFGetVillage((CTCVillager*)it->component);
        if (!village || !village->IsCookTime_00658bb9()) goto done;
    } while (result == 4);
done:
    Terminate();
}