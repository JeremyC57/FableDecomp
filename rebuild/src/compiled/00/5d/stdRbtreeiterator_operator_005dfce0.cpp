// std::_Rb_tree_iterator<std::pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, std::_Nonconst_traits<..> >::operator--() -- STLport 4.6 iterator predecessor of map<CCharString, CIVCountedPointer<CAnimComponentBase> > (_Rb_global<bool>::_M_decrement inlined)
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CIVCountedPointer@VCAnimComponentBase@@@@@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DFCE0, 79 bytes, thiscall, returns *this.
// Retail bytes:  mov eax,ecx ; push edi ; mov edi,[eax] ; cmp byte ptr [edi],0 ; jne L ; mov ecx,[edi+4] ; cmp [ecx+4],edi ; jne L ; mov edi,[edi+0Ch] ; mov [eax],edi ; pop edi ; ret ; L: push esi ; mov esi,[edi+8] ; test esi,esi ; je P ; (descend right from the left child) ; mov [eax],esi ; pop esi ; pop edi ; ret ; P: (climb while node is its parent's left) ; pop esi ; mov [eax],edx ; pop edi ; ret
// Identity: the landed insert_unique 0x005E2580 of the CAnimationEntry registry map (Ego_r.pdb map<CCharString,
//   CIVCountedPointer<CAnimComponentBase> >) is its only caller (callers.py) and calls it as that tree's iterator operator-- (NF80,
//   fable_map.h FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE), as 0x00755BF0 / 0x007AC920 are for their trees. Retail has no identical-code
//   folding, so each tree's operator-- is its own COMDAT.
// NF82 (q10992): the catalog name CRetailTreeIterator::Decrement was descriptive (a Dinkumware-shaped local iterator model); the
//   row is now the explicit instantiation of the STLport iterator's operator-- on stlport/fable_map.h (the landed 0x006ADFB0 recipe).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CIVCountedPointer.h"
class CAnimComponentBase;
namespace std {
template _Rb_tree_iterator<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, _Nonconst_traits<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > > >& _Rb_tree_iterator<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> >, _Nonconst_traits<pair<CCharString const, CIVCountedPointer<CAnimComponentBase> > > >::operator--();
}
