// std::vector<CHeroSoulsCreatureDef>::~vector() -- STLport 4.6 vector destructor: each element's virtual destructor over [_M_start, _M_finish), then the buffer freed
// Leaf: ??1?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F26F70, oracle length 50 bytes.
// Name (agentNF23, name-fix queue land1011da): agentFAM07: ~vector<CHeroSoulsCreatureDef> called by the renamed scalar deleting destructor 0x00F26F40 (graded RELOCATION_MATCH, shared engine headers); element stride of the body matches sizeof(CHeroSoulsCreatureDef); CHeroSoulsWaveDef::Creatures is Ego_r.pdb vector<CHeroSoulsCreatureDef>, and the body steps
//   0x34-byte polymorphic elements (sizeof(CHeroSoulsCreatureDef) in retail STLport layout), destroying each through vtable slot 0 with flag 0.
//   Was the generic stand-in CUnprovenMember_00f26f40 (??1CUnprovenMember_00f26f40@@QAE@XZ) / catalog CRetailVector_poly0x34::~CRetailVector_poly0x34.
// Callees: the element's virtual (scalar deleting) destructor, slot 0 with 0; free 0x00BFEA14 (allocator::deallocate, null-checked).
// Model: the shared STLport 4.6 vector (stlport/fable_containers.h); the explicit instantiation emits the destructor as its own COMDAT.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CHeroSoulsCreatureDef.h"
#include "stlport/fable_containers.h"
template std::vector<CHeroSoulsCreatureDef, std::allocator<CHeroSoulsCreatureDef> >::~vector();
