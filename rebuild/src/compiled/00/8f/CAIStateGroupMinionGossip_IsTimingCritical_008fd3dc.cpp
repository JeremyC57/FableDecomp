// Member getter at this+0x18. __fastcall this=ecx. pack(1) keeps the Critical at
// Leaf: ?IsTimingCritical@CAIStateGroup_MinionGossip_Methods@@QAEDXZ
// the retail byte offset (natural alignment would shift an odd one).
#include "engine/CAIStateGroup_MinionGossip.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CAIStateGroup_MinionGossip_Methods : CAIStateGroup_MinionGossip {
    char IsTimingCritical();
};
#pragma pack(pop)
char CAIStateGroup_MinionGossip_Methods::IsTimingCritical() { return this->Critical; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsTimingCritical@CAIStateGroup_MinionGossip@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
