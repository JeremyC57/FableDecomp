// std::list<CVertexGroupBlock>::operator=(const list& x) -- copy-assign the optimiser's block list: assign over the existing blocks pairwise, then erase the surplus or append copies of the rest
// Leaf: ??4?$list@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADD020, 0xAB bytes, thiscall, `ret 4`, returns this.
// Identity: Ego_r.pdb list<CVertexGroupBlock> (CVertexGroupBlockOptimiser::Blocks); the landed caller 0x00AD9F10 binds this spelling
//   (FABLE_RETAIL_STLP_LIST_ASSIGN_POPBACK_DECL). Element assignment is CVertexGroupBlock's implicit operator= (vector<unsigned long>
//   ::operator= 0x0045BC09 on +0, the map<unsigned long, long> tree operator= 0x0045C79E on +0xC); the tail helpers are list::erase(first,
//   last) 0x00ADC4E0 and _M_insert_dispatch 0x00ADCC80.
// Port (NF78, round 7; q10766 binder, NF70 blocked/port): stlport/fable_containers.h (NF78 overlay opt-in FABLE_RETAIL_STLP_LIST_ASSIGN_BODY:
//   the STLport _list.c operator= body and the declared erase(first, last); the existing LIST_INSERT_DISPATCH_NOINLINE keeps the range
//   insert's body visible but out of line) + stlport/fable_map.h + engine/CVertexGroupBlock.h (MEMBERS); the local namespace std and
//   CVertexGroupBlock models are gone. One explicit instantiation.
#define FABLE_RETAIL_STLP_LIST_ASSIGN_BODY
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#include "engine/CArray.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
#include "engine/CVertexGroupBlock.h"

template std::list<CVertexGroupBlock>& std::list<CVertexGroupBlock>::operator=(const std::list<CVertexGroupBlock>&);
