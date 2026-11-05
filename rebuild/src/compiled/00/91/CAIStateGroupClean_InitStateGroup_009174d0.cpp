// Leaf: ?InitStateGroup@CAIStateGroup_Clean@@QAEXHH@Z
struct CAIStateGroup_Clean { char pad[0x14]; unsigned char b14; unsigned char b15; char pad2[0x20-0x16]; int f20; unsigned char b24; void InitStateGroup(int world, int creature); };
void CAIStateGroup_Clean::InitStateGroup(int world, int creature){ this->b24=0; this->f20=0; this->b14=0; this->b15=0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?InitStateGroup@CAIStateGroup_Clean@@MAEXABVCWorld@@AAVCThingAICreature@@@Z); evidence: RTTI vtable slot + PDB name set.
