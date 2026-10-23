// 007ad060 _STL::_Rb_tree<CBasicString<char>*>::insert_unique
// Flags: /Gr
// Leaf: insert_unique
// Layout: tree Header+0; node parent+4/left+8/right+12/value+16; iterator one pointer, pair<iterator,bool>8 bytes.
namespace FableTree_007ad060 {
template<class C> struct CBasicString {C* Data;bool operator<(const CBasicString&)const;};
template<class C> __declspec(noinline) bool CBasicString<C>::operator<(const CBasicString& p)const{const C* a=Data;const C* b=p.Data;while(*a && *a==*b){++a;++b;}return *a<*b;}
template<class T> struct CComparePointer {__forceinline bool operator()(const T* a,const T* b)const{bool result;if(b==a)result=false;else if(!a)result=true;else if(!b)result=false;else result=*a<*b;return result;}};
struct _Rb_tree_node {int Color;_Rb_tree_node* Parent;_Rb_tree_node* Left;_Rb_tree_node* Right;CBasicString<char>* Value;};
struct _Rb_tree_iterator {
 _Rb_tree_node* Node;
 _Rb_tree_iterator(){} _Rb_tree_iterator(_Rb_tree_node* n):Node(n){}
 void _M_decrement();
};
template<class A,class B> struct pair {A first;B second;__forceinline pair(const A&a,const B&b):first(a),second(b){}};
struct _Rb_tree {
 _Rb_tree_node* Header;
 CComparePointer<CBasicString<char> > Compare;
 _Rb_tree_iterator _M_insert(_Rb_tree_node*,_Rb_tree_node*,CBasicString<char>*const&,bool);
 pair<_Rb_tree_iterator,bool> insert_unique(CBasicString<char>*const&);
};
pair<_Rb_tree_iterator,bool> _Rb_tree::insert_unique(CBasicString<char>*const& value){
 _Rb_tree_node* y=Header;_Rb_tree_node* x=Header->Parent;bool comp=true;
 while(x){y=x;comp=Compare(value,x->Value);x=comp?x->Left:x->Right;}
 _Rb_tree_iterator j(y);
 if(comp){if(j.Node==Header->Left)return pair<_Rb_tree_iterator,bool>(_M_insert(y,y,value,false),true);else j._M_decrement();}
 if(Compare(j.Node->Value,value))return pair<_Rb_tree_iterator,bool>(_M_insert(x,y,value,false),true);
 return pair<_Rb_tree_iterator,bool>(j,false);
}
}