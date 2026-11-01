// CFlammableDef::GetAllFireHelpers(std::vector<CDefString>& helpers) const -- push every key of the def's helper-point -> effect map (+0x40) onto the vector, in tree order
// Flags: /O2 /Oy /W3
// Leaf: ?GetAllFireHelpers@CFlammableDef@@QBEXAAV?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@@Z
// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb ?GetAllFireHelpers@CFlammableDef@@QBEXAAV?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@@Z --
//   the CFlammableDef (CTCFlammable::PDef) of InitFirePoints 0x007808B0, the same map the landed GetEffectAtHelperPoint 0x007817F0 walks;
//   was Row00781b90::TreeOwner::BuildVectorFromTree over a local vector<unsigned> (catalog CTCAvoidRegion_BuildVectorFromTree, propagated).
//   The vector is the row's inline STLport push_back model (three pointers; _M_insert_overflow), spelled as the std::vector instantiation.
#include <stdlib.h>
#include <new>
class CDefString { public: unsigned long CRC; };               // 4 bytes (the map key; Ego_r.pdb class V)
template <class K, class V> struct pair { K first; V second; };
namespace Row00781b90 {
// Layout: exact retail tree header +40, _M_node _M_color,_M_parent,_M_left,_M_right,_M_value_field +0,4,8,c,10.
// Layout: vector storage is three pointers; no shared CTCAvoidRegion header exists.
struct _Rb_tree_node {unsigned _M_color;_Rb_tree_node*_M_parent,*_M_left,*_M_right;pair<const CDefString, long> _M_value_field;};
struct _Rb_tree_iterator {
 _Rb_tree_node*_M_node;
 __forceinline explicit _Rb_tree_iterator(_Rb_tree_node*p):_M_node(p){}
 __forceinline const pair<const CDefString, long>&operator*()const{return _M_node->_M_value_field;}
 __forceinline bool operator!=(const _Rb_tree_iterator&other)const{return _M_node!=other._M_node;}
 __forceinline _Rb_tree_iterator&operator++(){
  _Rb_tree_node*x=_M_node;
  if(x->_M_right){x=x->_M_right;while(x->_M_left)x=x->_M_left;}
  else{_Rb_tree_node*y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}
  _M_node=x;return *this;
 }

};
template<class T> __forceinline const T&max(const T&a,const T&b){return a<b?b:a;}
template<class T> __forceinline void _Construct(T*out,const T&v){if(out)new(out)T(v);}
template<class T> __forceinline T*__uninitialized_copy(T*_M_start,T*_M_finish,T*out){for(;_M_start!=_M_finish;++_M_start,++out)_Construct(out,*_M_start);return out;}
}
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector { public:
 T*_M_start,*_M_finish,*_M_end_of_storage;
 __forceinline void _M_insert_overflow(const T&v,unsigned count){
   unsigned old=_M_finish-_M_start;unsigned cap=old+Row00781b90::max(old,count);
   T*oldLast=_M_finish;T*buffer=cap?(T*)malloc(cap*sizeof(T)):0;
   T*out=Row00781b90::__uninitialized_copy(_M_start,oldLast,buffer);
   for(unsigned i=0;i<count;++i){Row00781b90::_Construct(out,v);++out;}
   if(_M_start)free(_M_start);_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+cap;
 }
 __forceinline void push_back(const T&v){
  if(_M_finish!=_M_end_of_storage){Row00781b90::_Construct(_M_finish,v);++_M_finish;}
  else _M_insert_overflow(v,1);
 }

};
}
class CFlammableDef { public:
 char pad[0x40];Row00781b90::_Rb_tree_node*_M_header;      // +0x40: the helper-point -> effect map (STLport map<CDefString, long>, header pointer first)
 __forceinline Row00781b90::_Rb_tree_iterator begin()const{return Row00781b90::_Rb_tree_iterator(_M_header->_M_left);}
 __forceinline Row00781b90::_Rb_tree_iterator end()const{return Row00781b90::_Rb_tree_iterator(_M_header);}
 void GetAllFireHelpers(std::vector<CDefString>& helpers) const;
};
void CFlammableDef::GetAllFireHelpers(std::vector<CDefString>& helpers)const{for(Row00781b90::_Rb_tree_iterator it=begin();it!=end();++it)helpers.push_back((*it).first);}