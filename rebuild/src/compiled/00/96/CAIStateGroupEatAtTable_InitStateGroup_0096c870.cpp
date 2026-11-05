// Leaf: ?InitStateGroup@CAIStateGroup_EatAtTable@@QAEXHH@Z
struct CAIStateGroup_EatAtTable { char pad[0x48]; unsigned long f; void InitStateGroup(int world, int creature); };
void CAIStateGroup_EatAtTable::InitStateGroup(int world, int creature){ this->f = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_EatAtTable@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
