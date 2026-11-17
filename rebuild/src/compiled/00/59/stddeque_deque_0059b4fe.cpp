// std::deque<NUISystem::CComponent*>::deque(const deque& x) -- STLport 4.6 deque copy constructor: base built with the other's allocator and size, then the elements uninitialized-copied into the new storage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B4FE, 91 bytes. Manifest label "CShaderRenderManager::SetD3DMatrix" is propagated and
// rejected: the body returns `this` (a constructor), sizes a deque from two 16-byte iterators and copies the range.
// Callees: _Deque_iterator_base::_M_subtract @ 0x0059A8F3 (size() = finish - start through the inline operator-),
// _Deque_base(const allocator&, size_t) @ 0x0059AEF9 (ten zeroed words + _M_initialize_map), __uninitialized_copy(first, last,
// result, const __true_type&) @ 0x0059B49B (`__fastcall`: hidden iterator result in ecx, the tag in edx, the three iterators by
// value; the pointer element's is_POD_type is __true_type).
// Shape: a frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`, ends in `leave`): `this` spilled to [ebp-4]
// because esi/edi carry the three `movsd` iterator copies; the empty allocator and the __true_type temporaries both live in
// the dead high byte of the argument slot (`lea eax,[ebp+0Bh]`, `lea edx,[ebp+0Bh]`); the discarded returned iterator is the
// 16-byte frame temporary (`lea ecx,[ebp-14h]`).
// Identity (NF84, round 7, q11052 binder): its one landed caller NUISystem::CFrontEndManager::IsScreenOnStack (0x005959BA) copies the
//   screen-history deque<NUISystem::CComponent*> (the same instance as 0x0059A8F3 _M_subtract, 0x0059AEF9 _Deque_base(const allocator&,
//   size_t), 0x0059B61C push_back); the body is STLport 4.6's deque(const deque&). Descriptive by library (STLport; PDBs are Dinkumware).
//   Was the stand-in _unattributed::PointerDeque::PointerDeque (local iterator / base models; the size() call bound 0x0059A8F3 as
//   ??GSDequeIterator@@QBEJABU0@@Z).
// Port (NF84): stlport/fable_deque.h with the NF84 overlay opt-in FABLE_RETAIL_STLP_DEQUE_COPY_CTOR (+ NF79's STACK_SIZE_PUSH /
//   STACK_TOP_POP for the out-of-line _M_subtract), explicitly instantiated.
// Leaf: ??0?$deque@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@ABV01@@Z
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_STACK_SIZE_PUSH
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_STLP_DEQUE_COPY_CTOR
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template std::deque<NUISystem::CComponent*>::deque(const std::deque<NUISystem::CComponent*>& x);
