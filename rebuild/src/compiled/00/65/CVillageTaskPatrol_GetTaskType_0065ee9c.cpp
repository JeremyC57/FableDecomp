// Leaf: ?GetTaskType@CVillageTaskPatrol_Methods@@QAEHXZ
#pragma optimize("s",on)
// Constant-return member: returns 1. __fastcall this=ecx.
#include "engine/CVillageTaskPatrol.h"  // retyped onto the PDB layout; byte parity re-verified
struct CVillageTaskPatrol_Methods : CVillageTaskPatrol {
    int GetTaskType();
};
int CVillageTaskPatrol_Methods::GetTaskType() { return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetTaskType@CVillageTaskPatrol@@UBE?AW4EVillageTaskType@@XZ); evidence: RTTI vtable slot + PDB name set.
