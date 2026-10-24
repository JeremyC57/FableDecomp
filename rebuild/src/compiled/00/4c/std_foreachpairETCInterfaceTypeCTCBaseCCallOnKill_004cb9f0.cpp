// std::for_each<pair<ETCInterfaceType, CTCBase*>*, CCallOnKill>(first, last, f) -- STLport: call every thing component's OnKill (CTCBase virtual slot 14) across the component map, returning the functor
// Leaf: ??$for_each@PAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@VCCallOnKill@@@std@@YI?AVCCallOnKill@@PAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@0@0V1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CB9F0, length 64 bytes (49 + int3 padding), __fastcall (/Gr TU: ecx = first, edx = last,
// the 1-byte functor by value on the stack), `ret 4`, the functor returned in al.
// Identity: Ego_r.pdb `??$for_each@...@VCCallOnKill@@@std@@YI?AVCCallOnKill@@...` (Dinkumware iterator spelling; retail
// STLport passes raw pointers). Its single caller is the landed CThing::OnKill 0x004C7CF0
// (`for_each(Components.begin(), Components.end(), CCallOnKill())` over CVectorMap<ETCInterfaceType, CTCBase*> +0x44).
// The catalog "_Copy_backward<CPixelFormat*,CPixelFormat*>" is a propagated label.
// Callees: CTCBase virtual slot 14 (+0x38, OnKill).
// Shape: STLport `for (; first != last; ++first) f(*first); return f;`. CCallOnKill takes the component pair BY VALUE and
// the STLport pair has its user copy ctor (`first(o.first), second(o.second)`): the inlined copy keeps the pointer's
// store into the 8-byte frame (`mov [esp+0Ch],eax`) and drops the dead `first`; with the implicit (trivial) copy VC
// keeps `first` instead and calls through a direct load.
enum ETCInterfaceType { TCI_NULL };

class CTCBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012();
    virtual void OnDie();                       // slot 13 (+0x34)
    virtual void OnKill();                      // slot 14 (+0x38)
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};
}

class CCallOnKill {
public:
    void operator()(std::pair<ETCInterfaceType, CTCBase*> component) const { component.second->OnKill(); }
};

namespace std {
template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter first, _InputIter last, _Function f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

template CCallOnKill __fastcall for_each<pair<ETCInterfaceType, CTCBase*>*, CCallOnKill>(pair<ETCInterfaceType, CTCBase*>*,
                                                                                         pair<ETCInterfaceType, CTCBase*>*, CCallOnKill);
}