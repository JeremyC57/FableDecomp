// std::_Rb_tree_iterator<pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >, _Nonconst_traits<...> >::operator--() -- red-black tree iterator predecessor
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBW4EMode@NCameraModes@@V?$CCountedPointer@VCBaseInit@NCameraModes@@@@@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069CA00, 79 bytes, thiscall `ret`.
// Retail bytes:  mov eax,ecx ; push edi ; mov edi,[eax] ; cmp byte ptr [edi],0 ; jne L ; mov ecx,[edi+4] ; cmp [ecx+4],edi ; jne L ;
//   mov edi,[edi+0Ch] ; mov [eax],edi ; pop edi ; ret ; L: ... (STLport _Rb_global::_M_decrement inlined, the node stored back once).
// Identity (NF56, round 7, queue q10414): the map<NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > iterator's operator--,
//   the copy at this address (every instantiation emits a byte-identical body); the new NF41 binder 0x0069D900 (_Rb_tree::_M_insert of
//   that map) calls it at this address with this spelling. Was the generated stand-in CRetailTreeIterator::Decrement (no Leaf line).
//   The explicit instantiation of the shared stlport/fable_map.h iterator replaces the local node/iterator model.
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
namespace NCameraModes { class CBaseInit; enum EMode; }

template std::_Rb_tree_iterator<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >,
    std::_Nonconst_traits<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > > >&
std::_Rb_tree_iterator<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >,
    std::_Nonconst_traits<std::pair<const NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> > > >::operator--();
