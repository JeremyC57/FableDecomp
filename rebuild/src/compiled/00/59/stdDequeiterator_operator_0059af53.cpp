// std::_Deque_iterator<NUISystem::CComponent*, _Nonconst_traits<NUISystem::CComponent*> >::operator--() -- the out-of-line prefix
// decrement of the deque<NUISystem::CComponent*> iterator: _M_decrement (0x0059AC41), return *this
// Leaf: ??F?$_Deque_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059AF53, 12 bytes (`push esi ; mov esi,ecx ; call 0x0059AC41 ; mov eax,esi ; pop esi ; ret`),
//   a COMDAT in the deque<NUISystem::CComponent*> run (0x0059A8F3 .. 0x0059B61C); no E8 / E9 / absolute reference to it in Fable.exe.
// Identity (NF83, round 7, q11023 binder + own name): the body is STLport 4.6 _deque.h `_Self& operator--() { this->_M_decrement();
//   return *this; }` over the deque<NUISystem::CComponent*> iterator (its callee 0x0059AC41 is that iterator's _M_decrement).
//   DESCRIPTIVE-by-library: no PDB, string or caller names this address, and the traits argument (_Nonconst_traits, the deque's
//   `iterator`; _Const_traits would give the same bytes) is not proven by the bytes. Was the stand-in
//   _unattributed::CStringOwner_0059af53 (a constructor of a local CStringOwner calling the member ctor ??0CMember_0059ac41Member).
// Port (NF83): one explicit instantiation of the stlport/fable_deque.h operator-- (FABLE_STLPORT_DEQUE_PUSH_BACK carries it and the
//   inline _M_decrement; FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES' declaration-only _M_decrement keeps it a call).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_DEQUE_BACK_POP_BODIES
#include "stlport/fable_deque.h"
namespace NUISystem { class CComponent; }
template std::_Deque_iterator<NUISystem::CComponent*, std::_Nonconst_traits<NUISystem::CComponent*> >&
    std::_Deque_iterator<NUISystem::CComponent*, std::_Nonconst_traits<NUISystem::CComponent*> >::operator--();
