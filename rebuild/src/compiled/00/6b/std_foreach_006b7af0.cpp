// std::for_each(TYPE* first, TYPE* last, Function function) -- apply the member function to every element and return the functor.
// Leaf: ??$for_each@PAVCGameMapCell@@V?$mem_fun_ref_t@XVCGameMapCell@@@std@@@std@@YI?AV?$mem_fun_ref_t@XVCGameMapCell@@@0@PAVCGameMapCell@@0V10@@Z
// Identity: proven; CGameMap::Resize0x6B75A0 passes Reset0x6B7490; stride7; PDB _For_each<CGameMapCell*,mem_fun_ref_t<void,CGameMapCell>>. STLport name replaces Dinkumware _For_each.
#pragma pack(push,1)
class CGameMapCell { public: unsigned char AtmosThemes[3],AtmosBlends[2]; signed char SoundTheme; unsigned char Music; void Reset(); };
#pragma pack(pop)
namespace std {
template<class R,class T> class mem_fun_ref_t {
public: explicit mem_fun_ref_t(R(T::*p)()) : Function(p) {} R operator()(T& object) const { return (object.*Function)(); }
private: R(T::*Function)();
};
template<class Iter,class Function> Function __fastcall for_each(Iter first,Iter last,Function function) { for(;first!=last;++first) function(*first); return function; }
}
template std::mem_fun_ref_t<void,CGameMapCell> __fastcall std::for_each(CGameMapCell*,CGameMapCell*,std::mem_fun_ref_t<void,CGameMapCell>);