// 00ad5030 _STL::_Rb_tree::insert_unique
// Flags: none
// Leaf: ?insert_unique@_Rb_tree
#include <stdlib.h>
namespace RecutTree_00ad5030 {
// Layout: retail node allocator copies two dwords to+16/+20; mapped dword type is unproven.
namespace _STL {template<class T>struct less{__forceinline bool operator()(const T&a,const T&b)const{return a<b;}};template<class A,class B>struct pair {A first;B second;pair(){}pair(const A& a):first(a),second(){}pair(const A&a,const B&b):first(a),second(b){}pair(const pair&o):first(o.first),second(o.second){}};}
typedef _STL::pair<unsigned,float> value_type;
// Layout: retail node colour +0,parent +4,left +8,right +12,key +16; tree header +0,count +4.
struct _Rb_tree_node {unsigned char Colour;unsigned char Pad[3];_Rb_tree_node* Parent;_Rb_tree_node* Left;_Rb_tree_node* Right;unsigned Key;float Mapped;};
struct _Rb_tree_base_iterator{_Rb_tree_node*Node;};
struct _Rb_tree_iterator: _Rb_tree_base_iterator {
 __forceinline _Rb_tree_iterator(){}
 __forceinline _Rb_tree_iterator(_Rb_tree_node*p){Node=p;}
 __forceinline _Rb_tree_iterator(const _Rb_tree_iterator&o){Node=o.Node;}
 void _M_increment(); void _M_decrement();
};
__declspec(noinline) void _Rb_tree_iterator::_M_increment(){if(Node->Right){Node=Node->Right;while(Node->Left)Node=Node->Left;}else{_Rb_tree_node* p=Node->Parent;while(Node==p->Right){Node=p;p=p->Parent;}if(Node->Right!=p)Node=p;}}
__declspec(noinline) void _Rb_tree_iterator::_M_decrement(){if(Node->Colour==0 && Node->Parent->Parent==Node){Node=Node->Right;return;}if(Node->Left){Node=Node->Left;while(Node->Right)Node=Node->Right;}else{_Rb_tree_node* p=Node->Parent;while(Node==p->Left){Node=p;p=p->Parent;}Node=p;}}
typedef _STL::pair<_Rb_tree_iterator,bool> _Rb_tree_insert_result;
struct _Rb_tree {
 _Rb_tree_node* Header;unsigned Count;_STL::less<unsigned> _M_key_compare;
 _Rb_tree_iterator _M_insert(_Rb_tree_node*,_Rb_tree_node*,const value_type&, _Rb_tree_node*);
 _Rb_tree_insert_result insert_unique(const value_type&);
 _Rb_tree_iterator insert_unique(_Rb_tree_iterator,const value_type&);
};
void _Rb_global_rebalance(_Rb_tree_node*,_Rb_tree_node*&);
__declspec(noinline) _Rb_tree_iterator _Rb_tree::_M_insert(_Rb_tree_node*x,_Rb_tree_node*parent,const value_type&value,_Rb_tree_node*forceRight){bool left=parent==Header||(!forceRight&&(x||value.first<parent->Key));_Rb_tree_node*n=(_Rb_tree_node*)malloc(sizeof(_Rb_tree_node));n->Key=value.first;n->Mapped=value.second;n->Parent=parent;n->Left=n->Right=0;if(left){parent->Left=n;if(parent==Header){Header->Parent=n;Header->Right=n;}else if(parent==Header->Left)Header->Left=n;}else{parent->Right=n;if(parent==Header->Right)Header->Right=n;}_Rb_global_rebalance(n,Header->Parent);++Count;return _Rb_tree_iterator(n);}
__declspec(noinline) _Rb_tree_insert_result _Rb_tree::insert_unique(const value_type& value){_Rb_tree_node* p=Header;_Rb_tree_node* n=Header->Parent;while(n){p=n;if(value.first<n->Key)n=n->Left;else if(n->Key<value.first)n=n->Right;else{_Rb_tree_insert_result r;r.first.Node=n;r.second=false;return r;}}_Rb_tree_insert_result r;r.first=_M_insert(0,p,value,0);r.second=true;return r;}
_Rb_tree_iterator _Rb_tree::insert_unique(_Rb_tree_iterator position,const value_type& value){
 if(position.Node==Header->Left){
  if(Count<=0)
   return insert_unique(value).first;
  if(_M_key_compare(value.first,position.Node->Key))
   return _M_insert(position.Node,position.Node,value,0);
  else{
   bool comp_pos_v=_M_key_compare(position.Node->Key,value.first);
   if(comp_pos_v==false)
    return position;
   _Rb_tree_iterator after=position;
   after._M_increment();
   if(after.Node==Header)
    return _M_insert(0,position.Node,value,position.Node);
   if(_M_key_compare(value.first,after.Node->Key)){
    if(position.Node->Right==0)
     return _M_insert(0,position.Node,value,position.Node);
    else
     return _M_insert(after.Node,after.Node,value,0);
   }else{
    return insert_unique(value).first;
   }
  }
 }else if(position.Node==Header){
  if(_M_key_compare(Header->Right->Key,value.first))
   return _M_insert(0,Header->Right,value,position.Node);
  else
   return insert_unique(value).first;
 }else{
  _Rb_tree_iterator before=position;
  before._M_decrement();
  bool comp_v_pos=_M_key_compare(value.first,position.Node->Key);
  if(comp_v_pos&&_M_key_compare(before.Node->Key,value.first)){
   if(before.Node->Right==0)
    return _M_insert(0,before.Node,value,before.Node);
   else
    return _M_insert(position.Node,position.Node,value,0);
  }else{
   _Rb_tree_iterator after=position;
   after._M_increment();
   bool comp_pos_v=!comp_v_pos;
   if(!comp_v_pos)comp_pos_v=_M_key_compare(position.Node->Key,value.first);
   if((!comp_v_pos)&&comp_pos_v&&(after.Node==Header||_M_key_compare(value.first,after.Node->Key))){
    if(position.Node->Right==0)
     return _M_insert(0,position.Node,value,position.Node);
    else
     return _M_insert(after.Node,after.Node,value,0);
   }else{
    if(comp_v_pos==comp_pos_v)
     return position;
    else
     return insert_unique(value).first;
   }
  }
 }
}
}