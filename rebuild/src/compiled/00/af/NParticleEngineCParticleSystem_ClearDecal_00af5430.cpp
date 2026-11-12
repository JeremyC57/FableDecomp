// NParticleEngine::CParticleSystem::ClearDecal(list<CDecal*>::iterator& it) -- release every engine primitive of the decal *it:
//   for each parent group whose handle is set (CDecal::GetParentHandle copies it into a local handle; the walk stops at the first
//   empty group), ask the engine to clear each non-negative child primitive id of that group (CDecal::GetPrimID), then clear the
//   group's handle and id list (CDecal::ClearParentHandle); finally empty both decal arrays (CDecal::ClearAll).
// Leaf: ?ClearDecal@CParticleSystem@NParticleEngine@@QAEXAAV?$_List_iterator@V?$_List_val@PAVCDecal@NParticleEngine@@V?$allocator@PAVCDecal@NParticleEngine@@@std@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF5430, 607 bytes, thiscall `ret 4`; 5 call sites: DeleteDecal 0x00AF5690 (@0x00AF569A),
//   DeleteAllDecals 0x00AF5770 (@0x00AF5797), ClearAllDecals 0x00AF58D0 (@0x00AF58E9), CPSCDecalRenderer::AddToBoundingSphere
//   0x00B1DC70 (@0x00B1DFA8), CPSCDecalRenderer::PreparePrimitives 0x00B1F200 (@0x00B1FE56).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / CBaseClass::~CBaseClass 0x0099A2E0 (the local handle's base), CIEngine slot 30
//   ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) (+0x78), CEnginePersistentPrimitiveBase::NotifyReleased (slot 3,
//   +0x0C) and CIVCountedPointeeBase::Delete (slot 1, +0x04) through the inline CEnginePrimitiveHandle::Clear, the element's virtual
//   destructor (slot 0, flag 0) in the inline vector<CEnginePrimitiveHandle>::erase, std::__copy<CEnginePrimitiveHandle*,..,int>
//   0x00440260 (__fastcall, out of line), memmove 0x00BFEAE6 (vector<long>::clear), vector<CArray<long> >::erase 0x00AF7910.
// Shape: the 8-byte local handle gives retail's `and esp,-8` frame. The handle's counted pointer releases with the copy taken
//   inside the null test (FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER): with the default Release the group index stays in ebx
//   across the loop head instead of in its frame slot. __copy<CEnginePrimitiveHandle*> is declared (its landed body is
//   0x00440260) so that ParentHandleList.clear() expands inline as in retail while the copy stays a call.
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
#define FABLE_RETAIL_CDECAL
#define FABLE_RETAIL_CDECAL_CLEAR
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_CLEARDECAL
#include "engine/CParticleSystem.h"
#include "engine/CDecal.h"
#include "engine/CIEngine.h"
namespace std {
// STLport __copy for the handle vector: out of line in retail (0x00440260, landed), declaration only.
template<> CEnginePrimitiveHandle* __fastcall __copy<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, int>(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, const random_access_iterator_tag&, int*);
}
namespace NParticleEngine {
void CParticleSystem::ClearDecal(std::list<CDecal*>::iterator& it)
{
    CDecal* decal = *it;
    CEnginePrimitiveHandle handle;
    for (long group = 0; decal->GetParentHandle(group, handle); ++group) {
        if (PEngine && !handle.IsNull()) {
            long id;
            for (long index = 0; decal->GetPrimID(group, index, id); ++index)
                PEngine->ClearChildPrimitive(handle, id);
        }
        decal->ClearParentHandle(group);
    }
    decal->ClearAll();
}
}