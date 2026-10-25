// 009efb80 CCacheInfo_InsertOrFind
// Flags: /O2 /Oy /W3 
// Leaf: insert_unique
// Layout: header pointer at+0; RB node color,parent,left,right,key at0,4,8,c,10. CCharString is one pointer to CBasicString<char>.
namespace CacheTree {
template<class C>struct CBasicString{C*data;bool operator<(const CBasicString&)const;};
template<> __declspec(noinline) bool CBasicString<char>::operator<(const CBasicString&v)const {const char*a=data;const char*b=v.data;while(*a||*b){if(*a<*b)return true;if(*a>*b)return false;++a;++b;}return false;}
struct CCharString{CBasicString<char>*p;__forceinline bool operator<(const CCharString&v)const{bool result;if(v.p==p)result=false;else if(!p)result=true;else if(!v.p)result=false;else result=*p<*v.p;return result;}};
namespace std {
struct _Rb_tree_node{unsigned color;_Rb_tree_node*parent,*left,*right;CCharString key;unsigned value;};
struct _Rb_tree_iterator{_Rb_tree_node*node;void _M_decrement();};
struct pair{_Rb_tree_iterator first;bool second;pair(_Rb_tree_iterator a,bool b):first(a),second(b){}};
struct _Rb_tree{_Rb_tree_node*header;unsigned count;_Rb_tree_iterator _M_insert(_Rb_tree_node*,_Rb_tree_node*,const CCharString&,bool);pair insert_unique(const CCharString&);};
pair _Rb_tree::insert_unique(const CCharString&value){
 _Rb_tree_node*y=header;_Rb_tree_node*x=header->parent;bool comp=true;
 while(x){y=x;comp=value<x->key;x=comp?x->left:x->right;}
 _Rb_tree_iterator j;j.node=y;
 if(comp){if(j.node==header->left)return pair(_M_insert(y,y,value,false),true);j._M_decrement();}
 if(j.node->key<value)return pair(_M_insert(x,y,value,false),true);
 return pair(j,false);
}
}
}