// std::_Rb_tree::~_Rb_tree() -- STLport 4.6 tree destructor: clear() (out-of-line _M_erase from the root, header relinked, count zeroed), then _Rb_tree_base frees the header node (std::map<CTexture*, CEngineResourceManager::CExternalTextureAllocationInfo >)
// Leaf: ??1?$_Rb_tree@PAVCTexture@@U?$pair@QAVCTexture@@UCExternalTextureAllocationInfo@CEngineResourceManager@@@std@@U?$_Select1st@U?$pair@QAVCTexture@@UCExternalTextureAllocationInfo@CEngineResourceManager@@@std@@@3@U?$less@PAVCTexture@@@3@V?$allocator@U?$pair@QAVCTexture@@UCExternalTextureAllocationInfo@CEngineResourceManager@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3E8D0, 66 bytes.
// Name (agentNF23, name-fix queue land1011da, agentFAM07 proof): the instantiation's types are proven: agentFAM07: the out-of-line _M_erase the renamed destructor 0x00B3E9D0 calls (graded RELOCATION_MATCH this pass, its constructor 0x00B3E990 too); STLport 4.6 stl/_tree.h declares _Rb_tree::_M_erase private (the landed 0x00529CE0 template text), so VC7.1 spells it AAE; the tree type is that row's proven instantiation (RENAMES.tsv)
//   This row's own member was spelled over the per-instantiation placeholders CUnprovenKey/Value/KeyOfValue_00b3e6e0 (??1?$_Rb_tree@UCUnprovenKey_00b3e6e0@@UCUnprovenValue_00b3e6...);
// Identity (proven, agentFAM07): CEngineResourceManager's external-texture map (Ego_r.pdb map<CTexture*, CEngineResourceManager::CExternalTextureAllocationInfo>,
//   the class's only map): ~CEngineResourceManager 0x00B3B8E0 calls this tree's _M_erase 0x00B3E6E0 (0x00B3B957); ??0 / ??1 of this map
//   are in the PDB name sets; node 0x1C = 0x10 + 4 + 8 (Ego_r CExternalTextureAllocationInfo: long Handle, CXboxBufferMemoryPool* Pool).
// Model (agentNF24 port of agentNF23's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CEngineResourceManager.h (opt-in nested CExternalTextureAllocationInfo) replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES
class CTexture;
#include "stlport/fable_map.h"
#include "engine/CEngineResourceManager.h"

template std::_Rb_tree<CTexture*, std::pair<CTexture* const, CEngineResourceManager::CExternalTextureAllocationInfo>, std::_Select1st<std::pair<CTexture* const, CEngineResourceManager::CExternalTextureAllocationInfo> >, std::less<CTexture*>, std::allocator<std::pair<CTexture* const, CEngineResourceManager::CExternalTextureAllocationInfo> > >::~_Rb_tree();
