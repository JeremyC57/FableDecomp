// CParticleProgram::ClearGroups() -- clear simulated and non-simulated mesh groups.
// Leaf: ?ClearGroups@CParticleProgram@@QAEXXZ
// Retail: 0x00AE3510, 55 bytes, thiscall.
// Flags: none
// Identity: Ego_r 00BEF4A0 and FableWin 03336E90; donor clears both original group arrays in this order.
// Layout: original SimGroups/NonSimGroups members, native offsets +0xfc/+0x108.
// Calls: both vector<C3DGroup2>::erase calls bind 00AE7F30.
#define FABLE_RETAIL_CPARTICLEPROGRAM
#define FABLE_RETAIL_CPARTICLEPROGRAM_PASS22_CLEARGROUPS
#define FABLE_RETAIL_C3DGROUP2
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "engine/C3DGroup2.h"
#include "engine/CParticleProgram.h"

void CParticleProgram::ClearGroups()
{
    SimGroups.clear();
    NonSimGroups.clear();
}