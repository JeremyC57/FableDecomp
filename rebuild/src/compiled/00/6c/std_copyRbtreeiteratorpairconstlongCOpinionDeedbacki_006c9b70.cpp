// std::__copy<_Rb_tree_iterator<pair<const long, COpinionDeed*> >, back_insert_iterator<vector<pair<long, COpinionDeed*> > >, int>(first, last, back_insert_iterator result, const input_iterator_tag&, int*) -- STLport input-iterator copy: append every (handle, deed) entry of the deed map to the vector through push_back
// Leaf: ??$__copy@U?$_Rb_tree_iterator@U?$pair@$$CBJPAVCOpinionDeed@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJPAVCOpinionDeed@@@std@@@2@@std@@V?$back_insert_iterator@V?$vector@U?$pair@JPAVCOpinionDeed@@@std@@V?$allocator@U?$pair@JPAVCOpinionDeed@@@std@@@2@@std@@@2@H@std@@YI?AV?$back_insert_iterator@V?$vector@U?$pair@JPAVCOpinionDeed@@@std@@V?$allocator@U?$pair@JPAVCOpinionDeed@@@std@@@2@@std@@@0@U?$_Rb_tree_iterator@U?$pair@$$CBJPAVCOpinionDeed@@@std@@U?$_Nonconst_traits@U?$pair@$$CBJPAVCOpinionDeed@@@std@@@2@@0@0V10@ABUinput_iterator_tag@0@PAH@Z
// Grade: RELOCATION_MATCH target (NF74 port onto stlport/fable_containers.h + fable_map.h; was a whole local std model)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C9B70, length 187 bytes (+ int3 padding), `__fastcall` (hidden
// back_insert_iterator result in ecx, the tag reference in edx; first, last, result and the null Distance* on the stack =
// `ret 10h`, a /Gr algorithm TU). Callers: 0x006C7423 inside CTCHeroOpinionDeedLog::OnSerialise 0x006C7370 (manifest
// "CDeedManager::TransferState"; Ego_r.pdb OpinionDeedMap map<long, COpinionDeed*> is the retail +0x0C map it copies into a
// vector before writing it) and the two out-of-line `std::copy` instances 0x006C9D20 / 0x006C9DB0. The manifest label
// "BinaryTree_InsertPairs" is descriptive.
// Callees: vector<pair<long, COpinionDeed*> >::_M_insert_overflow(pointer, const value_type&, const __false_type&,
// size_type, bool) @ 0x006C80E0 (landed; pushed as `(finish, value, tag, 1, true)`).
// Layout: STLport tree node {colour, parent +4, left +8, right +0xC, value pair +0x10/+0x14}; vector {start, finish,
// end_of_storage}; back_insert_iterator {container pointer}.
// Shape: STLport 4.x `__copy` for input iterators verbatim -- `for ( ; first != last; ++result, ++first) *result =
// *first;` -- with back_insert_iterator::operator= -> push_back inline (the map's pair<const long, X> converted to the
// vector's pair<long, X> in a frame temporary, `_Construct` behind its null test, else _M_insert_overflow with the
// __false_type tag homed in the dead `result` slot) and the step `_M_node = _Rb_global<bool>::_M_increment(_M_node)`
// inline (the in-place member walk keeps the node in esi and loses retail's eax copy).
// Port (NF74, round 7): every STLport class comes from the shared headers -- pair, vector (_Vector_base, _STLP_alloc_proxy),
// back_insert_iterator (FABLE_RETAIL_STLP_BACK_INSERTER), _Rb_tree_iterator<V, _Nonconst_traits<V> > / _Rb_global (fable_map.h),
// and the input-iterator __copy itself (FABLE_RETAIL_STLP_COPY_INPUT_NOINLINE, NF62's opt-in for the caller 0x006C7370).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_COPY_INPUT_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

class COpinionDeed;

typedef std::pair<const long, COpinionDeed*> DeedMapValue;
typedef std::vector<std::pair<long, COpinionDeed*> > DeedVector;
typedef std::_Rb_tree_iterator<DeedMapValue, std::_Nonconst_traits<DeedMapValue> > DeedMapIterator;

template std::back_insert_iterator<DeedVector> __fastcall std::__copy<DeedMapIterator, std::back_insert_iterator<DeedVector>, int>(
    DeedMapIterator first, DeedMapIterator last,
    std::back_insert_iterator<DeedVector> result, const std::input_iterator_tag&, int*);
