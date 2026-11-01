// std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >::~pair() -- implicit destructor of a CScriptBase entity-script binding entry: release the counted binding (+0x04), then the name by tail jump
// Leaf: ??1?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB9700, 60 bytes, thiscall, `jmp` tail; no call or address reference (an unreferenced
//   COMDAT kept by the linker, no /OPT:REF).
// Identity: the script-manager TU around 0x00CB92C0..0x00CBBxxx works on two sorted vectors -- Ego_r.pdb CScriptBase
//   (+0x24) CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<..> > EntityScriptBindings
//   (its landed __median 0x00CBA620, operator= 0x00CBA700, swap 0x00CBA8E0 are pair<CCharString,
//   CCountedPointer<CEntityScriptBindingBase> >) and CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >
//   (_Uninit_fill_n 0x00CBA420, swap 0x00CBA7E0, _Push_heap 0x00CBABA0). Each element type is destroyed by name twice:
//   the implicit ~pair and STLport _Destroy(_Tp*). Retail has the two shapes in two adjacent couples, u64 pair first:
//   0x00CB96C0 / 0x00CB9700 among the class-template member COMDATs (after the ~CCountedPointer<..> bodies 0x00CB9560 /
//   0x00CB95F0 / 0x00CB9680), and 0x00CBA680 / 0x00CBA6C0 among the sort and heap function templates. VC7.1 emits the
//   implicit class members before the function-template instantiations that use them (lane experiment exp/order2.cpp:
//   both ~pair COMDATs precede both _Destroy<pair> COMDATs), so this first copy is ~pair. Order-derived (no caller): if a
//   later caller or EH table shows otherwise, swap with 0x00CBA6C0. Ego_r.pdb's ICF-folded _Dest_val of this pair (donor
//   0x00C9C2D0) names the type. The manifest label (_Dest_val<..C3DSkeletalMorphTarget..>) is propagated and wrong.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (the block),
//   CCharString::~CCharString 0x0099EAE0 (tail).
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};

class CEntityScriptBindingBase;

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

typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > TEntityScriptBindingPair;

void EmitEntityScriptBindingPairDestructor(TEntityScriptBindingPair* p)   // emits the implicit ??1; not a retail function
{
    p->TEntityScriptBindingPair::~TEntityScriptBindingPair();
}