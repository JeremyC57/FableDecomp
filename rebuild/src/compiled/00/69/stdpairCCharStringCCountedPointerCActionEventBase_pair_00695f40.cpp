// std::pair<CCharString,CCountedPointer<CActionEventBase> >::~pair() -- implicit destructor of the (non-const key) NCreatureAction action-event registry pair: release the counted event (+0x04), then the name by tail jump
// Leaf: ??1?$pair@VCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00695F40, 60 bytes, thiscall, `jmp` tail; no call or address reference (an unreferenced
//   COMDAT kept by the linker, no /OPT:REF).
// Identity: the CCreatureActionBase TU (0x006921B0..0x00696AF0). Its RegisterActionEventTypes 0x006932B0 builds, per event
//   class, a pair<CCharString, CCountedPointer<CActionEventBase> > (GetActionName() result + the counted event) at
//   [esp+0x1C], converts it to the map's value_type pair<const CCharString, CCountedPointer<CActionEventBase> > at [esp+0x28],
//   inserts it into the registry map (0x013BA8BC, insert 0x00696B70) and destroys both temporaries inline (const pair
//   first) -- so this TU destroys exactly three {CCharString, CCountedPointer<CActionEventBase>} types by name: ~pair<K,V>,
//   ~pair<const K,V> and STLport _Destroy<pair<const K,V> > (the map's node teardown). Retail holds exactly three bodies of
//   the shape in the TU: 0x00695F40 and 0x00695F80 adjacent among the class-template member COMDATs, and 0x006966D0 among
//   the function-template ones (beside 0x00696710, the same split for pair<unsigned long, CCountedPointer<CFaction> > whose
//   ~pair 0x00695D90 is called from 0x006F78xx). Lane experiment (exp/order1.cpp, VC7.1 RTM): the implicit ~pair<K,V> is
//   emitted immediately before ~pair<const K,V>, both before any function-template instantiation such as _Destroy. So this
//   first copy is ~pair<CCharString, CCountedPointer<CActionEventBase> >; Ego_r.pdb holds that pair type
//   (_Pair_base<CCharString,CCountedPointer<NCreatureAction::CActionEventBase>>) next to the const one. Order-derived:
//   if a later caller or EH table shows otherwise, swap with 0x00695F80. The manifest label (_Dest_val<..pair<CCharString
//   const,CCountedPointer<C3DSkeletalMorphTarget>>..>) is the Ego_r ICF representative of the shape and wrong here.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (the block),
//   CCharString::~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ, tail).
// Shape: pair declares no destructor; second's ~CCountedPointer (engine/CCountedPointer.h: re-read Info after the
//   decrement, Data = 0 then Info = 0) runs first, first's ~CCharString is the tail jmp.
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
}

typedef std::pair<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > TActionEventNamePair;

void EmitActionEventNamePairDestructor(TActionEventNamePair* p)     // emits the implicit ??1; not a retail function
{
    p->TActionEventNamePair::~TActionEventNamePair();
}