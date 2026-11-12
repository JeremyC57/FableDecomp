// std::map<long, std::pair<long, long> >::~map() -- STLport 4.6 container destructor (implicit): the member _Rb_tree's destructor inlined -- clear(), then free the header node
// Leaf: ??1?$map@JU?$pair@JJ@std@@U?$less@J@2@V?$allocator@U?$pair@$$CBJU?$pair@JJ@std@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006172B0, 66 bytes.
// Types (proven, agentFAM11, 2026-11-12): map<long, pair<long, long> > (node 0x1C = 0x10 + 4 + 8, signed key). Sources: (1) retail: CTCInventoryMap::CreateMapMarkers 0x005FF190 calls
//   this tree's _M_erase 0x00613CE0 (its local per-region counter map), and the same TU's landed _M_insert 0x00616F90 is this map's
//   (map<long, pair<long,long> >::operator[] 0x00618DE0 -> insert_unique 0x00617D50); the ~_Rb_tree 0x00616A80 / ~map 0x006172B0 call 0x00613CE0;
//   (2) FableWin.pdb ??0 / ??1 / ??A of map<long, pair<long,long> > and Ego_r.pdb ??A (donor 0x00668CB0), both in the CTCInventoryMap TU.
//   Was the placeholder CUnprovenContainer_00613ce0::~CUnprovenContainer_00613ce0 (leaf ??1CUnprovenContainer_00613ce0@@QAE@XZ); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / map; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

// The implicit ~map (STLport declares none) inlines ~_Rb_tree: clear(), then free the header node. Retail emitted it out
//   of line; this emitter (not part of the graded body) makes the TU emit that COMDAT.
void DestroyMapOfLongPairLongLong(std::map<long, std::pair<long, long> >* container) { container->~map(); }
