// 00781b90 Row00781b90::TreeOwner::Build
// Flags: /O2 /Oy /W3
// Leaf: Build
#include <stdlib.h>
#include <new>
namespace Row00781b90 {
// Layout: exact retail tree header +40, node color,parent,left,right,value +0,4,8,c,10.
// Layout: vector storage is three pointers; no shared CTCAvoidRegion header exists.
struct Node {unsigned color;Node*parent,*left,*right;unsigned value;};
struct Iterator {
 Node*node;
 __forceinline explicit Iterator(Node*p):node(p){}
 __forceinline unsigned&operator*()const{return node->value;}
 __forceinline bool operator!=(const Iterator&other)const{return node!=other.node;}
 __forceinline Iterator&operator++(){
  Node*x=node;
  if(x->right){x=x->right;while(x->left)x=x->left;}
  else{Node*y=x->parent;while(x==y->right){x=y;y=y->parent;}if(x->right!=y)x=y;}
  node=x;return *this;
 }

};
template<class T> __forceinline const T&Max(const T&a,const T&b){return a<b?b:a;}
template<class T> __forceinline void Construct(T*out,const T&v){if(out)new(out)T(v);}
template<class T> __forceinline T*UninitializedCopy(T*first,T*last,T*out){for(;first!=last;++first,++out)Construct(out,*first);return out;}
template<class T> struct RetailVector {
 T*first,*last,*limit;
 __forceinline void Grow(const T&v,unsigned count){
   unsigned old=last-first;unsigned cap=old+Max(old,count);
   T*oldLast=last;T*buffer=cap?(T*)malloc(cap*sizeof(T)):0;
   T*out=UninitializedCopy(first,oldLast,buffer);
   for(unsigned i=0;i<count;++i){Construct(out,v);++out;}
   if(first)free(first);first=buffer;last=out;limit=buffer+cap;
 }
 __forceinline void Push(const T&v){
  if(last!=limit){Construct(last,v);++last;}
  else Grow(v,1);
 }

};
typedef RetailVector<unsigned> Vector;
struct TreeOwner {
 char pad[0x40];Node*head;
 __forceinline Iterator begin(){return Iterator(head->left);}
 __forceinline Iterator end(){return Iterator(head);}
 void Build(Vector*);
};
void TreeOwner::Build(Vector*out){for(Iterator it=begin();it!=end();++it)out->Push(*it);}
}