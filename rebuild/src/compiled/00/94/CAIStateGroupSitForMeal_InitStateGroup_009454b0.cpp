// Leaf: ?InitStateGroup@CAIStateGroup_SitForMeal@@QAEXABVCWorld@@AAVCThingAICreature@@@Z
class CWorld; class CThingAICreature;
struct CAIStateGroup_SitForMeal { char pad[0x3c]; int f3c; unsigned char b40; char pad2[3]; int f44; void InitStateGroup(const CWorld&, CThingAICreature&); };
void CAIStateGroup_SitForMeal::InitStateGroup(const CWorld&, CThingAICreature&){ b40=0; f3c=-1; f44=-1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_SitForMeal@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
