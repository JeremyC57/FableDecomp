// 00781b90 Row00781b90::TreeOwner::BuildVectorFromTree (catalog CTCAvoidRegion_BuildVectorFromTree; no CTCAvoidRegion class in the PDBs, owner kept)
// Flags: /O2 /Oy /W3
// Leaf: BuildVectorFromTree
#include <stdlib.h>
#include <new>
namespace Row00781b90 {
// Layout: exact retail tree header +40, _M_node _M_color,_M_parent,_M_left,_M_right,_M_value_field +0,4,8,c,10.
// Layout: vector storage is three pointers; no shared CTCAvoidRegion header exists.
struct _Rb_tree_node {unsigned _M_color;_Rb_tree_node*_M_parent,*_M_left,*_M_right;unsigned _M_value_field;};
struct _Rb_tree_iterator {
 _Rb_tree_node*_M_node;
 __forceinline explicit _Rb_tree_iterator(_Rb_tree_node*p):_M_node(p){}
 __forceinline unsigned&operator*()const{return _M_node->_M_value_field;}
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
template<class T> struct vector {
 T*_M_start,*_M_finish,*_M_end_of_storage;
 __forceinline void _M_insert_overflow(const T&v,unsigned count){
   unsigned old=_M_finish-_M_start;unsigned cap=old+max(old,count);
   T*oldLast=_M_finish;T*buffer=cap?(T*)malloc(cap*sizeof(T)):0;
   T*out=__uninitialized_copy(_M_start,oldLast,buffer);
   for(unsigned i=0;i<count;++i){_Construct(out,v);++out;}
   if(_M_start)free(_M_start);_M_start=buffer;_M_finish=out;_M_end_of_storage=buffer+cap;
 }
 __forceinline void push_back(const T&v){
  if(_M_finish!=_M_end_of_storage){_Construct(_M_finish,v);++_M_finish;}
  else _M_insert_overflow(v,1);
 }

};
typedef vector<unsigned> Vector;
struct TreeOwner {
 char pad[0x40];_Rb_tree_node*_M_header;
 __forceinline _Rb_tree_iterator begin(){return _Rb_tree_iterator(_M_header->_M_left);}
 __forceinline _Rb_tree_iterator end(){return _Rb_tree_iterator(_M_header);}
 void BuildVectorFromTree(Vector*);
};
void TreeOwner::BuildVectorFromTree(Vector*out){for(_Rb_tree_iterator it=begin();it!=end();++it)out->push_back(*it);}
}