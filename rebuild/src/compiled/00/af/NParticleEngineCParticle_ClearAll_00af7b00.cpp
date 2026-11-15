// NParticleEngine::CParticle::ClearAll() -- release parent handles and clear every primitive-id group.
// Leaf: ?ClearAll@CParticle@NParticleEngine@@QAEXXZ
// Retail: 0x00AF7B00, 55 bytes, thiscall.
// Flags: none
// Identity: FableWin.pdb 02F18EA0; its body clears ParentHandleList then PrimIDList.
// Layout: Ego_r member order and native +0x90/+0x84 accesses agree with the shared header.
// Calls: vector<CEnginePrimitiveHandle>::erase at 00440D70; vector<CArray<long> >::erase at 00AF7910.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CQUATERNION_MULASSIGN
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CPARTICLE_PASS22_CLEARALL
#include "engine/CParticle.h"

void NParticleEngine::CParticle::ClearAll()
{
    ParentHandleList.clear();
    PrimIDList.clear();
}