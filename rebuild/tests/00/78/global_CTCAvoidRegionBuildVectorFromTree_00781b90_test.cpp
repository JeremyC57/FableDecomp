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

#include <stdio.h>
#include <assert.h>
#include <string.h>
int main(){
 using namespace Row00781b90;
 TreeOwner owner;Node head,nodes[7];Vector values;
 memset(&owner,0,sizeof(owner));memset(&head,0,sizeof(head));
 memset(nodes,0,sizeof(nodes));memset(&values,0,sizeof(values));
 owner.head=&head;head.left=&head;owner.Build(&values);
 assert(values.first==0&&values.last==0&&values.limit==0);
 // Balanced tree exercises descent through a left child and repeated ascent.
 for(unsigned i=0;i<7;++i)nodes[i].value=i+1;
 head.parent=&nodes[3];head.left=&nodes[0];head.right=&nodes[6];
 nodes[3].parent=&head;nodes[3].left=&nodes[1];nodes[3].right=&nodes[5];
 nodes[1].parent=&nodes[3];nodes[1].left=&nodes[0];nodes[1].right=&nodes[2];
 nodes[5].parent=&nodes[3];nodes[5].left=&nodes[4];nodes[5].right=&nodes[6];
 nodes[0].parent=&nodes[1];nodes[2].parent=&nodes[1];
 nodes[4].parent=&nodes[5];nodes[6].parent=&nodes[5];
 owner.Build(&values);assert(values.last-values.first==7);
 for(unsigned j=0;j<7;++j)assert(values.first[j]==j+1);
 owner.Build(&values);assert(values.last-values.first==14);
 for(unsigned k=0;k<14;++k)assert(values.first[k]==k%7+1);
 free(values.first);
 // Exact available capacity traverses without allocation or replacing storage.
 values.first=(unsigned*)malloc(7*sizeof(unsigned));assert(values.first);
 values.last=values.first;values.limit=values.first+7;unsigned*storage=values.first;
 owner.Build(&values);assert(values.first==storage&&values.last==values.limit);
 for(unsigned m=0;m<7;++m)assert(values.first[m]==m+1);
 free(values.first);
 values.first=(unsigned*)malloc(sizeof(unsigned));values.last=values.first+1;values.limit=values.last;values.first[0]=9;unsigned repeated=99;values.Grow(repeated,2);assert(values.last-values.first==3&&values.limit-values.first==3&&values.first[0]==9&&values.first[1]==99&&values.first[2]==99);free(values.first);
 puts("00781b90_TEST PASS");return 0;
}