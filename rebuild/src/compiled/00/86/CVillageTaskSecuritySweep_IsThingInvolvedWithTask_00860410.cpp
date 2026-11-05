// Leaf: ?IsThingInvolvedWithTask@CVillageTaskSecuritySweep@@QAE_NJ@Z
#include "rebuild_abi.h"
// CVillageTaskSecuritySweep::IsThingInvolved @ 0x00860410
// add ecx,0x8; call helper; sub eax,[esp+4]; neg;sbb;inc; ret 4
// Return whether the current assignee (helper over the +0x8 subobject) equals arg.
struct CVillageTaskSecuritySweepSub;
extern "C" long FABLE_FASTCALL FableAssignee_00860410(CVillageTaskSecuritySweepSub *sub);
class CVillageTaskSecuritySweep { public: char pad00[0x8]; char m_sub[4]; bool IsThingInvolvedWithTask(long who); };
bool CVillageTaskSecuritySweep::IsThingInvolvedWithTask(long who)
{
    return FableAssignee_00860410((CVillageTaskSecuritySweepSub*)&m_sub[0]) == who;
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsThingInvolvedWithTask@CVillageTaskSecuritySweep@@UBE_NABVCThing@@@Z); evidence: RTTI vtable slot + PDB name set.
