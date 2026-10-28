// std::for_each(TYPE* first, TYPE* last, Function function) -- apply the member function to every element and return the functor.
// Leaf: ??$for_each@PAVCHeightMapCell@@V?$mem_fun_ref_t@XVCHeightMapCell@@@std@@@std@@YI?AV?$mem_fun_ref_t@XVCHeightMapCell@@@0@PAVCHeightMapCell@@0V10@@Z
// Identity: proven; CHeightMap::Resize0x8F61A0 calls at0x8F6226 with Reset0x8F5EB0; stride12; matching PDB mem_fun_ref_t instantiation. STLport name replaces Dinkumware _For_each.
// Layout: shared CHeightMapCell data layout reused under an alias solely to add its missing member declaration; all data members remain from the shared header.
#define CHeightMapCell CHeightMapCell_HeaderLayout
#include "engine/CHeightMapCell.h"
#undef CHeightMapCell
class CHeightMapCell : public CHeightMapCell_HeaderLayout { public: void Reset(); };
namespace std {
template<class R,class T> class mem_fun_ref_t {
public: explicit mem_fun_ref_t(R(T::*p)()) : Function(p) {} R operator()(T& object) const { return (object.*Function)(); }
private: R(T::*Function)();
};
template<class Iter,class Function> Function __fastcall for_each(Iter first,Iter last,Function function) { for(;first!=last;++first) function(*first); return function; }
}
template std::mem_fun_ref_t<void,CHeightMapCell> __fastcall std::for_each(CHeightMapCell*,CHeightMapCell*,std::mem_fun_ref_t<void,CHeightMapCell>);