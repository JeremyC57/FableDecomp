// std::__uninitialized_copy(const_iterator first, const_iterator last, iterator result, const __true_type&) -- (deque<NUISystem::CComponent*>)
//   STLport 4.6 _uninitialized.h: `return __copy_aux(first, last, result, _BothPtrType<..>::_Ret())` (deque iterators: __false_type)
// Leaf: ??$__uninitialized_copy@U?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Const_traits@PAVCComponent@NUISystem@@@std@@@std@@U?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@2@@std@@YI?AU?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@0@U?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Const_traits@PAVCComponent@NUISystem@@@std@@@0@0U10@ABU__true_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B49B, 61 bytes, __fastcall (hidden result in ecx, the tag in edx, three 16-byte iterators by value), `ret 30h`.
// Was the stand-in CStaticMeshPlacement_0059b49b::CStaticMeshPlacement_0059b49b (a "constructor" over a local 16-byte struct; the callee bound as
//   ?InitPlacement_0059b2cc@@..). Its caller 0x0059B4FE (NF84) already binds this spelling.
// TU (NF86, round 7, q11104): retail's deque<NUISystem::CComponent*> copy-constructor TU (0x0059AFD7 .. 0x0059B576, `/Gr` size TU): the
//   copy constructor 0x0059B4FE calls __uninitialized_copy(x.begin(), x.end(), _M_start, __true_type) 0x0059B49B out of line, which calls
//   __copy_aux(.., __false_type) 0x0059B2CC, which calls the random-access __copy 0x0059AFD7; each is a __fastcall COMDAT (hidden result in ecx).
//   One source for the three rows: stlport/fable_deque.h (NF84 FABLE_RETAIL_STLP_DEQUE_COPY_CTOR + NF79 STACK_SIZE_PUSH/TOP_POP for the iterator
//   operator- / _M_subtract 0x0059A8F3, as 0x0059B4FE) with the NF86 overlay opt-in FABLE_RETAIL_STLP_DEQUE_UNINIT_COPY_BODY, and
//   stlport/fable_containers.h's STLport __copy_aux dispatch (FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22); the explicit instantiation of
//   __uninitialized_copy emits all three as pick-any COMDATs. The input iterators are const_iterator (x is const), so __copy is
//   <_Const_traits, _Nonconst_traits>. Descriptive by library (STLport 4.6; both PDBs are Dinkumware builds).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
#define FABLE_RETAIL_STLP_STACK_SIZE_PUSH
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_STLP_DEQUE_COPY_CTOR
#define FABLE_RETAIL_STLP_DEQUE_UNINIT_COPY_BODY
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
typedef std::_Deque_iterator<NUISystem::CComponent*, std::_Nonconst_traits<NUISystem::CComponent*> > ComponentDequeIterator;
typedef std::_Deque_iterator<NUISystem::CComponent*, std::_Const_traits<NUISystem::CComponent*> > ComponentDequeConstIterator;
template ComponentDequeIterator FABLE_FASTCALL std::__uninitialized_copy(ComponentDequeConstIterator, ComponentDequeConstIterator, ComponentDequeIterator, const std::__true_type&);
