// std::map<CTexture*, CEngineResourceManager::CExternalTextureAllocationInfo >::~map() -- STLport 4.6 container destructor (implicit): the member _Rb_tree's destructor inlined -- clear() (the out-of-line _M_erase), then free the header node
// Leaf: ??1?$map@PAVCTexture@@UCExternalTextureAllocationInfo@CEngineResourceManager@@U?$less@PAVCTexture@@@std@@V?$allocator@U?$pair@QAVCTexture@@UCExternalTextureAllocationInfo@CEngineResourceManager@@@std@@@5@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3E9D0, 66 bytes; node 0x1c.
// Identity (proven, agentFAM07): CEngineResourceManager's external-texture map (Ego_r.pdb map<CTexture*, CEngineResourceManager::CExternalTextureAllocationInfo>,
//   the class's only map): ~CEngineResourceManager 0x00B3B8E0 calls this tree's _M_erase 0x00B3E6E0 (0x00B3B957); ??0 / ??1 of this map
//   are in the PDB name sets; node 0x1C = 0x10 + 4 + 8 (Ego_r CExternalTextureAllocationInfo: long Handle, CXboxBufferMemoryPool* Pool).
//   Was the placeholder CUnprovenContainer_00b3e6e0::~CUnprovenContainer_00b3e6e0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Callee: _M_erase 0x00B3E6E0 (private, out of line), free 0x00BFEA14.
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CEngineResourceManager.h (opt-in nested CExternalTextureAllocationInfo) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES
class CTexture;
#include "stlport/fable_map.h"
#include "engine/CEngineResourceManager.h"

// The implicit ~map (STLport declares none) inlines ~_Rb_tree: clear(), then free the header node. Retail emitted it out
//   of line; this emitter (agentFAM07's TU device, not part of the graded body) makes the TU emit that COMDAT.
void DestroyMapOfCTextureCEngineResourceManagerCExternalTextureAllocationInfo(std::map<CTexture*, CEngineResourceManager::CExternalTextureAllocationInfo>* container) { container->~map(); }
