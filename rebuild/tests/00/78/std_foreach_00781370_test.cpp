#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
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

static int calls=0; void NFire::CFirePoint::Reset() { ++calls; HelperIndex+=1; }
typedef NFire::CFirePoint Element;
int main() { Element cells[4]; int i; for(i=0;i<4;++i) cells[i].HelperIndex=10+i;
 std::mem_fun_ref_t<void,Element> fn(&Element::Reset);
 std::mem_fun_ref_t<void,Element> result=std::for_each(cells,cells+3,fn);
 if(calls!=3 || cells[0].HelperIndex!=11 || cells[1].HelperIndex!=12 || cells[2].HelperIndex!=13 || cells[3].HelperIndex!=13) goto fail;
 result(cells[3]); if(calls!=4 || cells[3].HelperIndex!=14) goto fail;
 std::for_each(cells,cells,fn); if(calls!=4) goto fail;
 puts("00781370_TEST PASS"); return 0;
fail: puts("00781370_TEST FAIL"); return 1;
}