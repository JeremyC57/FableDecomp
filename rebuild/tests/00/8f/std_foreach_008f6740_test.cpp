#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
#pragma pack(push,1)
class CHeightMapCell { public: float Height; unsigned char EngineThemePaletteIndex[3],EngineBlend[2],NavPreferred[3]; void Reset(); };
#pragma pack(pop)
namespace std {
template<class R,class T> class mem_fun_ref_t {
public: explicit mem_fun_ref_t(R(T::*p)()) : Function(p) {} R operator()(T& object) const { return (object.*Function)(); }
private: R(T::*Function)();
};
template<class Iter,class Function> Function __fastcall for_each(Iter first,Iter last,Function function) { for(;first!=last;++first) function(*first); return function; }
}
template std::mem_fun_ref_t<void,CHeightMapCell> __fastcall std::for_each(CHeightMapCell*,CHeightMapCell*,std::mem_fun_ref_t<void,CHeightMapCell>);

static int calls=0; void CHeightMapCell::Reset() { ++calls; Height+=1; }
typedef CHeightMapCell Element;
int main() { Element cells[4]; int i; for(i=0;i<4;++i) cells[i].Height=10+i;
 std::mem_fun_ref_t<void,Element> fn(&Element::Reset);
 std::mem_fun_ref_t<void,Element> result=std::for_each(cells,cells+3,fn);
 if(calls!=3 || cells[0].Height!=11 || cells[1].Height!=12 || cells[2].Height!=13 || cells[3].Height!=13) goto fail;
 result(cells[3]); if(calls!=4 || cells[3].Height!=14) goto fail;
 std::for_each(cells,cells,fn); if(calls!=4) goto fail;
 puts("008f6740_TEST PASS"); return 0;
fail: puts("008f6740_TEST FAIL"); return 1;
}