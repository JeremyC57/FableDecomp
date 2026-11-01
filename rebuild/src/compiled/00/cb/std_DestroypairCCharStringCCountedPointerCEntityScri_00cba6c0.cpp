// std::_Destroy<pair<CCharString,CCountedPointer<CEntityScriptBindingBase> > >(pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >* __pointer) -- STLport single-element destroy of a CScriptBase entity-script binding entry: the inlined implicit ~pair
// Leaf: ??$_Destroy@U?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBA6C0, 60 bytes, __fastcall (pointer in ecx), `jmp` tail; no call or address
//   reference (an unreferenced COMDAT kept by the linker, no /OPT:REF).
// Identity: the script-manager TU's sorted vector<pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > >
//   (Ego_r.pdb CScriptBase::EntityScriptBindings, a CVectorMap with CCompareString; landed __median 0x00CBA620 right
//   before this row, operator= 0x00CBA700 right after it). Each of the TU's two pair element types is destroyed by name
//   twice, ~pair and STLport _Destroy(_Tp*); the second couple 0x00CBA680 (pair<unsigned __int64,
//   CCountedPointer<CActiveEntityScriptBase> >) / 0x00CBA6C0 sits among the sort and heap function templates, the first
//   couple 0x00CB96C0 / 0x00CB9700 among the class-template members. VC7.1 emits function-template instantiations after
//   the implicit members they use (lane experiment exp/order2.cpp), so this copy is _Destroy<pair<..> >. The TU builds /Gr
//   (the __median 0x00CBA620 is __fastcall). Order-derived (no caller): swap with 0x00CB9700 if a caller shows otherwise.
//   The manifest label (_Dest_val<..C3DSkeletalMorphTarget..>) is propagated and wrong.
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
// STLport 4.6 stl/_construct.h
template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
    __pointer;
    __pointer->~_Tp();
}
template void _Destroy<pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > >(pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >*);
}