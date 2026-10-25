// _STL::const_mem_fun1_t<bool,CThing,CCharString>::operator()(const CThing* p, CCharString x) const -- STLport const member-function adaptor: copy the by-value script name and call the bound `bool (CThing::*)(CCharString) const` (CThing::IsSameScriptName) on p
// Leaf: ??R?$const_mem_fun1_t@_NVCThing@@VCCharString@@@_STL@@QBE_NPBVCThing@@VCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048B2F0, 41 bytes, thiscall (this = the adaptor; p and the 4-byte
//   by-value CCharString on the stack), `ret 8`, returns the bool in al.
// Identity: FableWin.pdb's only const_mem_fun1_t over CCharString is `const_mem_fun1_t<bool, CThing, CCharString>`
//   (bind2nd(mem_fun(&CThing::IsSameScriptName), name) in a find_if over a vector<CThingHolySite*>), and its only
//   matching CThing member is `?IsSameScriptName@CThing@@QBE_NVCCharString@@@Z`. No call or absolute reference
//   reaches 0x0048B2F0 in retail (an unreferenced COMDAT copy). The catalog name `std::allocator_traits_Construct`
//   (and the autoname's pair<EHeroMorphType, CParticleMorphs::CEntry> guess) is wrong.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, ~CCharString() @ 0x0099EAE0; the member call is
//   `call [esi]` through the 4-byte single-inheritance pointer to member `_M_f` at +0.
// Shape: STLport 4.6 `_Ret operator()(const _Tp* __p, _Arg __x) const { return (__p->*_M_f)(__x); }`: the copy of x is
//   built in the pushed outgoing slot, the result byte parked in bl across x's destructor.
class CCharString {
public:
    void* PStringData;                                      // +0x0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};

class CThing {
public:
    bool IsSameScriptName(CCharString name) const;          // FableWin.pdb ?IsSameScriptName@CThing@@QBE_NVCCharString@@@Z
};

namespace _STL {
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

template <class _Ret, class _Tp, class _Arg>
class const_mem_fun1_t : public binary_function<const _Tp*, _Arg, _Ret> {
    typedef _Ret (_Tp::*__fun_type)(_Arg) const;
public:
    explicit const_mem_fun1_t(__fun_type __pf) : _M_f(__pf) { }
    _Ret operator()(const _Tp* __p, _Arg __x) const { return (__p->*_M_f)(__x); }
private:
    __fun_type _M_f;                                        // +0x0
};

template class const_mem_fun1_t<bool, CThing, CCharString>;
}