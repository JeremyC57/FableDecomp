// NParticleEngine::CParticle::SetPrimitiveInfo(long list, long index, const CEnginePrimitiveHandle& parent, long id) -- grow the per-list
//   primitive-ID table and the parent-handle list as needed, store the ID, then assign the parent handle.
// Leaf: ?SetPrimitiveInfo@CParticle@NParticleEngine@@QAEXJJABVCEnginePrimitiveHandle@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE9B0, 282 bytes (`ret 10h`): the out-of-line copy of the inline member.
// Identity (NF56, round 7, seen while porting this row's q10427 binding): FableWin.pdb 0x02F2A9A0
//   ?SetPrimitiveInfo@CParticle@NParticleEngine@@QAEXJJABVCEnginePrimitiveHandle@@J@Z; engine/CParticle.h (FABLE_RETAIL_CPARTICLE_SPRITE)
//   holds the body over the PDB members PrimIDList (+132, vector<CArray<long> >) and ParentHandleList (+144, vector<CEnginePrimitiveHandle>),
//   the offsets the old row proved from the retail accesses; four stack arguments match `ret 10h`. Callees: the outer resize(n, const
//   CArray<long>&) 0x00AFE910, the CArray<long> resize(n, const long&) 0x004428F0 and vector<CEnginePrimitiveHandle>::resize(size_type)
//   0x00443F90 (STLport spelling of the self row, q10427). Was the stand-in CParticleNative::SetPrimitiveID over local handle / counted
//   pointer / array models (catalog ClearChildPrimitive, a propagated label).
// The member is defined inline in the class; taking its address in the Kill_ helper makes VC7.1 emit the out-of-line COMDAT (the
//   helper is not part of the retail body).
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPARTICLE_SPRITE
#include "engine/CParticle.h"

typedef void (NParticleEngine::CParticle::*SetPrimitiveInfoMember)(long, long, const CEnginePrimitiveHandle&, long);
SetPrimitiveInfoMember Kill_00afe9b0()
{
    return &NParticleEngine::CParticle::SetPrimitiveInfo;
}
