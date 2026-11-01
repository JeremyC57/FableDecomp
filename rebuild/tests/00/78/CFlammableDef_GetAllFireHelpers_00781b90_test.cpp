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

#include <stdio.h>
#include <assert.h>
#include <string.h>
int main(){
 using namespace Row00781b90;
 TreeOwner owner;_Rb_tree_node _M_header,nodes[7];Vector values;
 memset(&owner,0,sizeof(owner));memset(&_M_header,0,sizeof(_M_header));
 memset(nodes,0,sizeof(nodes));memset(&values,0,sizeof(values));
 owner._M_header=&_M_header;_M_header._M_left=&_M_header;owner.BuildVectorFromTree(&values);
 assert(values._M_start==0&&values._M_finish==0&&values._M_end_of_storage==0);
 // Balanced tree exercises descent through a _M_left child and repeated ascent.
 for(unsigned i=0;i<7;++i)nodes[i]._M_value_field=i+1;
 _M_header._M_parent=&nodes[3];_M_header._M_left=&nodes[0];_M_header._M_right=&nodes[6];
 nodes[3]._M_parent=&_M_header;nodes[3]._M_left=&nodes[1];nodes[3]._M_right=&nodes[5];
 nodes[1]._M_parent=&nodes[3];nodes[1]._M_left=&nodes[0];nodes[1]._M_right=&nodes[2];
 nodes[5]._M_parent=&nodes[3];nodes[5]._M_left=&nodes[4];nodes[5]._M_right=&nodes[6];
 nodes[0]._M_parent=&nodes[1];nodes[2]._M_parent=&nodes[1];
 nodes[4]._M_parent=&nodes[5];nodes[6]._M_parent=&nodes[5];
 owner.BuildVectorFromTree(&values);assert(values._M_finish-values._M_start==7);
 for(unsigned j=0;j<7;++j)assert(values._M_start[j]==j+1);
 owner.BuildVectorFromTree(&values);assert(values._M_finish-values._M_start==14);
 for(unsigned k=0;k<14;++k)assert(values._M_start[k]==k%7+1);
 free(values._M_start);
 // Exact available capacity traverses without allocation or replacing storage.
 values._M_start=(unsigned*)malloc(7*sizeof(unsigned));assert(values._M_start);
 values._M_finish=values._M_start;values._M_end_of_storage=values._M_start+7;unsigned*storage=values._M_start;
 owner.BuildVectorFromTree(&values);assert(values._M_start==storage&&values._M_finish==values._M_end_of_storage);
 for(unsigned m=0;m<7;++m)assert(values._M_start[m]==m+1);
 free(values._M_start);
 values._M_start=(unsigned*)malloc(sizeof(unsigned));values._M_finish=values._M_start+1;values._M_end_of_storage=values._M_finish;values._M_start[0]=9;unsigned repeated=99;values._M_insert_overflow(repeated,2);assert(values._M_finish-values._M_start==3&&values._M_end_of_storage-values._M_start==3&&values._M_start[0]==9&&values._M_start[1]==99&&values._M_start[2]==99);free(values._M_start);
 puts("00781b90_TEST PASS");return 0;
}