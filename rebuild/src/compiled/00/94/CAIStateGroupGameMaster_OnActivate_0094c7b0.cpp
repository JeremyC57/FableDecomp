// Leaf: ?OnActivate@CAIStateGroup_GameMaster@@QAEXXZ
#include "rebuild_abi.h"
struct D { char p[0x1c9]; unsigned char f1c9; };
struct A { char p[0x18]; D* f18; };
struct CAIStateGroup_GameMaster { char p[4]; A* PBrain; char pac[0xac-8]; unsigned char WasAllowedToFollowPlayer; void OnActivate(); };
void CAIStateGroup_GameMaster::OnActivate(){ this->WasAllowedToFollowPlayer = (unsigned char)(this->PBrain->f18->f1c9 & 1); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnActivate@CAIStateGroup_GameMaster@@MAEXXZ); evidence: RTTI vtable slot + PDB name set.
