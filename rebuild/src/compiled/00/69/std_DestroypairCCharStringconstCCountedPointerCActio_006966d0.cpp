// std::_Destroy<pair<CCharString const,CCountedPointer<CActionEventBase> > >(pair<CCharString const,CCountedPointer<CActionEventBase> >* __pointer) -- STLport single-element destroy of an NCreatureAction action-event registry map value: the inlined implicit ~pair
// Leaf: ??$_Destroy@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@std@@YIXPAU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006966D0, 60 bytes, __fastcall (pointer in ecx), `jmp` tail; no call or address
//   reference (an unreferenced COMDAT kept by the linker, no /OPT:REF).
// Identity: the CCreatureActionBase TU (0x006921B0..0x00696AF0) keeps the registry map<CCharString,
//   CCountedPointer<NCreatureAction::CActionEventBase> > (Ego_r.pdb type; RegisterActionEventTypes 0x006932B0 inserts
//   CActionEventTriggerParticle, vtable 0x0125C0D8, etc.; its _M_erase 0x00696900 inlines this release + ~CCharString per
//   node at +0x10). The TU destroys three {CCharString, CCountedPointer<CActionEventBase>} types by name -- ~pair<K,V>,
//   ~pair<const K,V> (0x00695F40 / 0x00695F80, adjacent, among the class-template member COMDATs) and STLport
//   _Destroy<value_type> (map node teardown). This copy sits among the function-template COMDATs right before
//   0x00696710 (the same _Destroy for the TU's pair<unsigned long, CCountedPointer<CFaction> > map, whose own ~pair is
//   0x00695D90) and the two _M_erase bodies 0x00696900 / 0x00696970 in the same CCharString-then-unsigned-long order.
//   VC7.1 emits function-template instantiations after the implicit class members they use (lane experiments
//   exp/order1.cpp, exp/order2.cpp), so this is _Destroy<pair<const CCharString, CCountedPointer<CActionEventBase> > >.
//   STLport 4.6 _Destroy(_Tp*) is the retail spelling of Ego_r's Dinkumware _Dest_val (chunk 656, 0x00CD4FA0); the TU
//   builds /Gr (its for_each 0x00695B20 is __fastcall). The manifest label (_Dest_val<..C3DSkeletalMorphTarget..>) is the
//   Ego_r ICF representative of the shape and wrong here.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (the block),
//   CCharString::~CCharString 0x0099EAE0 (tail).
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};

namespace NCreatureAction { class CActionEventBase; }

namespace std {
// STLport 4.6 stl/_pair.h (the members this row needs)
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
// STLport 4.6 stl/_construct.h
template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
    __pointer;
    __pointer->~_Tp();
}
template void _Destroy<pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > >(pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >*);
}