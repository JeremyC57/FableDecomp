// 00801ec0 BinaryTree_Insert_Upper
// Flags: none
// Leaf: ?insert_unique@_Rb_tree
namespace SignedKeyTree {
// Layout: retail node allocator copies two dwords to+16/+20; mapped dword type is unproven.
namespace std {template<class A,class B>struct pair {A first;B second;pair(const A& a):first(a),second(){}};}
typedef std::pair<const int,unsigned> value_type;
// Layout: retail node colour +0,parent +4,left +8,right +12,key +16; tree header +0,count +4.
struct _Rb_tree_node_base {unsigned char Colour;unsigned char Pad[3];_Rb_tree_node_base*Parent,*Left,*Right;__forceinline const value_type&GetValue()const;};
struct _Rb_tree_node:_Rb_tree_node_base {value_type Value;_Rb_tree_node(const value_type&v=value_type(0)):Value(v){Colour=0;Parent=Left=Right=0;}};
__forceinline const value_type&_Rb_tree_node_base::GetValue()const{return static_cast<const _Rb_tree_node*>(this)->Value;}

struct _Rb_tree_base_iterator {
 _Rb_tree_node_base* Node;
 void _M_increment(); void _M_decrement();
};
struct _Rb_tree_iterator:_Rb_tree_base_iterator {__forceinline _Rb_tree_iterator(){}__forceinline _Rb_tree_iterator(_Rb_tree_node_base*p){Node=p;}__forceinline _Rb_tree_iterator(const _Rb_tree_iterator&i){Node=i.Node;}__forceinline _Rb_tree_iterator&operator++(){_M_increment();return *this;}__forceinline _Rb_tree_iterator&operator--(){_M_decrement();return *this;}};
__declspec(noinline) void _Rb_tree_base_iterator::_M_increment(){if(Node->Right){Node=Node->Right;while(Node->Left)Node=Node->Left;}else{_Rb_tree_node_base* p=Node->Parent;while(Node==p->Right){Node=p;p=p->Parent;}if(Node->Right!=p)Node=p;}}
__declspec(noinline) void _Rb_tree_base_iterator::_M_decrement(){if(Node->Colour==0 && Node->Parent->Parent==Node){Node=Node->Right;return;}if(Node->Left){Node=Node->Left;while(Node->Right)Node=Node->Right;}else{_Rb_tree_node_base* p=Node->Parent;while(Node==p->Left){Node=p;p=p->Parent;}Node=p;}}
struct _Rb_tree_insert_result { _Rb_tree_iterator First;bool Second;};
void __cdecl _Rb_tree_rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);
struct _Rb_tree {
 _Rb_tree_node_base*_M_create_node(const value_type&);
 _Rb_tree_node_base* Header;unsigned Count;
 _Rb_tree_iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const value_type&, _Rb_tree_node_base*);
 _Rb_tree_insert_result insert_unique(const value_type&);
 _Rb_tree_iterator insert_unique(_Rb_tree_iterator,const value_type&);
};
__declspec(noinline) _Rb_tree_iterator _Rb_tree::_M_insert(_Rb_tree_node_base*x,_Rb_tree_node_base*parent,const value_type&value,_Rb_tree_node_base*rightHint){_Rb_tree_node_base*n;if(parent==Header||(!rightHint&&(x||value.first<parent->GetValue().first))){n=_M_create_node(value);parent->Left=n;if(parent==Header){Header->Parent=n;Header->Right=n;}else if(parent==Header->Left)Header->Left=n;}else{n=_M_create_node(value);parent->Right=n;if(parent==Header->Right)Header->Right=n;}n->Left=n->Right=0;n->Parent=parent;_Rb_tree_rebalance(n,Header->Parent);++Count;return _Rb_tree_iterator(n);}
_Rb_tree_iterator _Rb_tree::insert_unique(_Rb_tree_iterator position,const value_type& value){
 if(position.Node==Header->Left){
  if(Count<=0)
   return insert_unique(value).First;
  if((value.first<position.Node->GetValue().first))
   return _M_insert(position.Node,position.Node,value,0);
  else{
   bool comp_pos_v=(position.Node->GetValue().first<value.first);
   if(comp_pos_v==false)
    return position;
   _Rb_tree_iterator after=position;
   after._M_increment();
   if(after.Node==Header)
    return _M_insert(0,position.Node,value,position.Node);
   if((value.first<after.Node->GetValue().first)){
    if(position.Node->Right==0)
     return _M_insert(0,position.Node,value,position.Node);
    else
     return _M_insert(after.Node,after.Node,value,0);
   }else{
    return insert_unique(value).First;
   }
  }
 }else if(position.Node==Header){
  if((Header->Right->GetValue().first<value.first))
   return _M_insert(0,Header->Right,value,position.Node);
  else
   return insert_unique(value).First;
 }else{
  _Rb_tree_iterator before=position;
  before._M_decrement();
  bool comp_v_pos=(value.first<position.Node->GetValue().first);
  if(comp_v_pos&&(before.Node->GetValue().first<value.first)){
   if(before.Node->Right==0)
    return _M_insert(0,before.Node,value,before.Node);
   else
    return _M_insert(position.Node,position.Node,value,0);
  }else{
   _Rb_tree_iterator after=position;
   after._M_increment();
   bool comp_pos_v=!comp_v_pos;
   if(!comp_v_pos)comp_pos_v=(position.Node->GetValue().first<value.first);
   if((!comp_v_pos)&&comp_pos_v&&(after.Node==Header||(value.first<after.Node->GetValue().first))){
    if(position.Node->Right==0)
     return _M_insert(0,position.Node,value,position.Node);
    else
     return _M_insert(after.Node,after.Node,value,0);
   }else{
    if(comp_v_pos==comp_pos_v)
     return position;
    else
     return insert_unique(value).First;
   }
  }
 }
}
}