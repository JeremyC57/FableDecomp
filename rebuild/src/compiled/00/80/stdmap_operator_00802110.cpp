// std::map::operator[](const EIdleStateGroup& key) -- STLport map<EIdleStateGroup, CAIStateGroupBase*> subscript: the mapped state group for the key, inserting a null entry first when the key is absent
// Leaf: ??A?$map@W4EIdleStateGroup@@PAVCAIStateGroupBase@@U?$less@W4EIdleStateGroup@@@std@@V?$allocator@U?$pair@$$CBW4EIdleStateGroup@@PAVCAIStateGroupBase@@@std@@@4@@std@@QAEAAPAVCAIStateGroupBase@@ABW4EIdleStateGroup@@@Z
// Grade: RELOCATION_MATCH -- chunk-422 recipe: insert_unique(hint, v) 0x00801CB0 receives the pair temporary's address; a same-TU noinline stand-in that READS v but never keeps its address lets VC7.1 schedule the default value's stores as retail (the parked "pair zero-store scheduling" residue). A stand-in that ignores v lets VC drop the pair stores (101 bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00802110, 115 bytes (+ int3 padding), __fastcall, `ret 4`, returns the
//   address of the mapped value. Callers: CTCIdleScheduler::RegisterIdleStateGroup 0x00801920 (`IdleStates[group] =
//   state` on this+0x0C) and two more in the CTCIdleScheduler TU (0x00801C15 / 0x00801C34).
// Callees: _Rb_tree::insert_unique(iterator, const value_type&) 0x00801CB0 (landed; iterator returned through a hidden
//   pointer, `ret 0Ch`). The sibling instantiation for map<EIdleStateGroup, long> is 0x00802190.
// Shape: STLport 4.x `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i = insert(i,
//   value_type(k, _Tp())); return (*i).second;` with lower_bound inlined (`!(key(x) < k)` keeps y = x and goes left; the
//   key is loaded once before the walk, signed `jl`). The insert's result iterator lands in the dead `k` argument slot,
//   the by-value hint iterator is copy-constructed in its argument slot, and the pair's key is stored before the pushes
//   while the zero `_Tp()` is stored after them.
// Types: Ego_r.pdb CTCIdleScheduler::IdleStates is map<EIdleStateGroup, CAIStateGroupBase*> (node key +0x10, value
//   +0x14); retail STLport trees hold the header node pointer first.
// NF61 (round 7, q10581): moved onto stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[] body, PASS22 __default_constructed,
//   FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY: the visible out-of-line hinted
//   insert_unique that reads the value, which schedules the pair temporary's late second-word store as retail -- the 0x00553E60 /
//   0x00A4A670 pattern); the local std::map model and its same-TU insert_unique stand-in are gone. The call binds the tree's
//   insert_unique(iterator, const value_type&) NF58 landed; the self leaf is the 4-argument STLport spelling. The landed caller 0x00801920 already binds that spelling; 0x00801BE0 binds the stand-in CIdleStateMap (NAME_FIXES); no landed caller bound the old 2-argument self spelling. EIdleStateGroup / CAIStateGroupBase are forward declarations (NF58 0x00801CB0 spelling; was a local enum with an invented enumerator).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
enum EIdleStateGroup;
class CAIStateGroupBase;
namespace std {
template CAIStateGroupBase*& map<EIdleStateGroup, CAIStateGroupBase*>::operator[](const EIdleStateGroup&);
}
