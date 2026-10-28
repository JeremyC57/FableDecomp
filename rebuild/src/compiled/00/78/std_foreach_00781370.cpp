// std::for_each(TYPE* first, TYPE* last, Function function) -- apply the member function to every element and return the functor.
// Leaf: ??$for_each@PAVCFirePoint@NFire@@V?$mem_fun_ref_t@XVCFirePoint@NFire@@@std@@@std@@YI?AV?$mem_fun_ref_t@XVCFirePoint@NFire@@@0@PAVCFirePoint@NFire@@0V10@@Z
// Identity: proven; CTCFlammable::ExtinguishFire0x780C80 calls at0x780C95 with CFirePoint::ExtinguishFire0x780700; FirePoints+16; stride44; matching PDB mem_fun_ref_t instantiation. STLport name replaces Dinkumware _For_each.
class CThing;
class CCharString { public: void* PStringData; };
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer() {} void* PData; };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {};
namespace NFire { class CFirePoint { public: CCharString HelperName; long HelperIndex,EffectIndex,Resistance; bool Burning; long FireDamageAfterFrame; CThing* PParentThing; CIntelligentPointer<CThing> PEffect,PSingeEffect; void Reset(); }; }
namespace std {
template<class R,class T> class mem_fun_ref_t {
public: explicit mem_fun_ref_t(R(T::*p)()) : Function(p) {} R operator()(T& object) const { return (object.*Function)(); }
private: R(T::*Function)();
};
template<class Iter,class Function> Function __fastcall for_each(Iter first,Iter last,Function function) { for(;first!=last;++first) function(*first); return function; }
}
template std::mem_fun_ref_t<void,NFire::CFirePoint> __fastcall std::for_each(NFire::CFirePoint*,NFire::CFirePoint*,std::mem_fun_ref_t<void,NFire::CFirePoint>);