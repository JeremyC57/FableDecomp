// std::vector::resize(size_type n, const value_type& x) -- shrink the trivial tail or fill-insert at end.
// Leaf: ?resize@?$vector@VCMapwhoLevelInfo@NThingMap@@V?$allocator@VCMapwhoLevelInfo@NThingMap@@@std@@@std@@QAEXIABVCMapwhoLevelInfo@NThingMap@@@Z
// Identity: proven; CThingMap::InitMapwhoLevels0085D180 calls this resize20; PDB CMapwhoLevelInfo field sequence Size/Extents/ScaleShift/NoThings confirms.
// Layout: no shared element header; actual PDB class keys and named POD fields; vector retail three-pointer layout. Trivial tail destruction folds away.
#ifndef _MSC_VER
#define __fastcall
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE __declspec(noinline)
#endif
class C2DExtentsI {public:long XExtent,YExtent;};namespace NThingMap {class CMapwhoLevelInfo {public:long Size;C2DExtentsI Extents;long ScaleShift,NoThings;};}
namespace std {
struct random_access_iterator_tag {}; template<class T> class allocator {}; template<class V,class Al> class _STLP_alloc_proxy:public Al { public: V _M_data; }; template<class T,class Al> class _Vector_base { public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,Al> _M_end_of_storage; };
template<class Iter,class Out,class Distance> NOINLINE Out __fastcall __copy(Iter first,Iter last,Out result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n,++first,++result)*result=*first;return result; }
template<class T,class Al=allocator<T> > class vector:public _Vector_base<T,Al> { public: typedef T* iterator; typedef unsigned int size_type; iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;} size_type size()const{return size_type(this->_M_finish-this->_M_start);} void _M_fill_insert(iterator,unsigned int,const T&); iterator erase(iterator first,iterator last){iterator result=__copy(last,end(),first,random_access_iterator_tag(),(int*)0);this->_M_finish=result;return first;}void resize(size_type n,const T& x){if(n<size()){erase(begin()+n,end());}else _M_fill_insert(end(),n-size(),x);} };
}
typedef NThingMap::CMapwhoLevelInfo Element;
template void std::vector<Element>::resize(unsigned int,const Element&);