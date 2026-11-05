// Leaf: ?InitStateGroup@CAIStateGroup_ServeFood@@QAEXHH@Z
#include "rebuild_abi.h"
struct CAIStateGroup_ServeFood {
    char a[0x14]; int CarriedFoodItems; char b[0x1c-0x18]; int Seat; char c[0x44-0x20]; int ServeActionPoint;
    void InitStateGroup(int world, int creature);
};
void CAIStateGroup_ServeFood::InitStateGroup(int world, int creature){ this->CarriedFoodItems=0; this->Seat=-1; this->ServeActionPoint=-1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_ServeFood@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
