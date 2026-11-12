// std::_Rb_tree<NUISystem::ETableSprites, std::pair<NUISystem::ETableSprites const, long>, std::_Select1st<std::pair<NUISystem::ETableSprites const, long> >, std::less<NUISystem::ETableSprites>, std::allocator<std::pair<NUISystem::ETableSprites const, long> > >::_Rb_tree(const std::less<NUISystem::ETableSprites>&, const std::allocator<std::pair<NUISystem::ETableSprites const, long> >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@W4ETableSprites@NUISystem@@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@U?$_Select1st@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@4@U?$less@W4ETableSprites@NUISystem@@@4@V?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@4@@std@@QAE@ABU?$less@W4ETableSprites@NUISystem@@@1@ABV?$allocator@U?$pair@$$CBW4ETableSprites@NUISystem@@J@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00630CC0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<NUISystem::ETableSprites, long> (agentFAM07 proved this tree for 0x00630F50 map<NUISystem::ETableSprites, long>::map, RENAMES 2026-11-11): NUISystem::CUIDef::Sprites (Ego_r.pdb map<NUISystem::ETableSprites, long>): ~CUIDef 0x006301A0 calls this tree's _M_erase 0x00630A10 on the member at +0x64 (0x006302D9); the erase destroys no value (rules out ActionMap, map<unsigned long, CCharString>), and the landed ETableSprites tree member 0x00631220 calls the same _M_erase (0x00631242).
//   This _Rb_tree constructor 0x00630CC0 is the COMDAT right before the tree's ~_Rb_tree 0x00630D00, which calls that _M_erase 0x00630A10.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00630a10,CUnprovenValue_00630a10,CUnprovenKeyOfValue_00630a10,std::less<CUnprovenKey_00630a10>,std::allocator<CUnprovenValue_00630a10> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00630a10@@UCUnprovenValue_00630a10@@UCUnprovenKeyOfValue_00630a10@@U?$less@UCUnprovenKey_00630a10@@@std@@V?$allocator@UCUnprovenValue_00630a10@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00630a10@@@1@ABV?$allocator@UCUnprovenValue_00630a10@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP) and engine/NUISystem_CUIDef.h; the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_C2DVECTOR
#include "stlport/fable_map.h"
#include "engine/NUISystem_CUIDef.h"

template std::_Rb_tree<NUISystem::ETableSprites, std::pair<NUISystem::ETableSprites const, long>, std::_Select1st<std::pair<NUISystem::ETableSprites const, long> >, std::less<NUISystem::ETableSprites>, std::allocator<std::pair<NUISystem::ETableSprites const, long> > >::_Rb_tree(const std::less<NUISystem::ETableSprites>&, const std::allocator<std::pair<NUISystem::ETableSprites const, long> >&);
