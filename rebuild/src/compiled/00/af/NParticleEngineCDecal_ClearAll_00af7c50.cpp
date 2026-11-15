// NParticleEngine::CDecal::ClearAll() -- release parent handles and clear every primitive-id group.
// Leaf: ?ClearAll@CDecal@NParticleEngine@@QAEXXZ
// Retail: 0x00AF7C50, 37 bytes, thiscall.
// Flags: none
// Identity: FableWin.pdb 02F19740; its body clears ParentHandleList then PrimIDList.
// Layout: Ego_r member order and native +0x68/+0x5c accesses agree with the shared header.
// Calls: vector<CEnginePrimitiveHandle>::erase at 00440D70; vector<CArray<long> >::erase at 00AF7910.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CDECAL
#define FABLE_RETAIL_CDECAL_PASS22_CLEARALL
#include "engine/CDecal.h"

void NParticleEngine::CDecal::ClearAll()
{
    ParentHandleList.clear();
    PrimIDList.clear();
}