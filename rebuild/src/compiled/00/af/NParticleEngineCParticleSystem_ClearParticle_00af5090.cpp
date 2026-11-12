// NParticleEngine::CParticleSystem::ClearParticle(list<CParticle*>::iterator& it) -- release every engine primitive of the particle *it:
//   for each parent group whose handle is set (CParticle::GetParentHandle copies it into a local handle; the walk stops at the first
//   empty group), and when the engine exists and the handle is live: ask the engine to clear each non-negative child primitive id of
//   that group (CParticle::GetPrimID), then clear the group's handle and id list (CParticle::ClearParentHandle; retail's PEngine/null
//   test jumps past it, 0x00AF515B -> 0x00AF5256); finally empty both particle arrays (CParticle::ClearAll).
// Leaf: ?ClearParticle@CParticleSystem@NParticleEngine@@QAEXAAV?$_List_iterator@V?$_List_val@PAVCParticle@NParticleEngine@@V?$allocator@PAVCParticle@NParticleEngine@@@std@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF5090, 657 bytes, thiscall `ret 4`; callers DeleteParticle 0x00AF5330, ClearAllParticles 0x00AF58A0, ...
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / CBaseClass::~CBaseClass 0x0099A2E0 (the local handle's base), CIEngine slot 30
//   ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) (+0x78), CEnginePersistentPrimitiveBase::NotifyReleased (slot 3,
//   +0x0C) and CIVCountedPointeeBase::Delete (slot 1, +0x04) through the inline CEnginePrimitiveHandle::Clear, the element's virtual
//   destructor (slot 0, flag 0) in the inline vector<CEnginePrimitiveHandle>::erase, std::__copy<CEnginePrimitiveHandle*,..,int>
//   0x00440260 (__fastcall, out of line), memmove 0x00BFEAE6 (vector<long>::clear), vector<CArray<long> >::erase 0x00AF7910.
// Shape: twin of ClearDecal 0x00AF5430 (landed). The 8-byte local handle gives retail's `and esp,-8` frame.
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DTOR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CPARTICLE_CLEARPARTICLE
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#include "engine/CParticleSystem.h"
#include "engine/CParticle.h"
#include "engine/CIEngine.h"
namespace std {
// STLport __copy for the handle vector: out of line in retail (0x00440260, landed), declaration only.
template<> CEnginePrimitiveHandle* __fastcall __copy<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, int>(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, const random_access_iterator_tag&, int*);
}
namespace NParticleEngine {
void CParticleSystem::ClearParticle(std::list<CParticle*>::iterator& it)
{
    CParticle* particle = *it;
    CEnginePrimitiveHandle handle;
    for (long group = 0; particle->GetParentHandle(group, handle); ++group) {
        if (PEngine && !handle.IsNull()) {
            long id;
            for (long index = 0; particle->GetPrimID(group, index, id); ++index)
                PEngine->ClearChildPrimitive(handle, id);
            particle->ClearParentHandle(group);
        }
    }
    particle->ClearAll();
}
}