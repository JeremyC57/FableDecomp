// 005e2580 std::_Rb_tree<CBasicString<char>*>::insert_unique
// Flags: /Gr
// Leaf: ?insert_unique@_Rb_tree@FableTree_005e2580@@QAE?AU?$pair@U_Rb_tree_iterator@FableTree_005e2580@@_N@2@ABQAU?$CBasicString@D@2@@Z
// Layout: tree Header+0; node parent+4/left+8/right+12/value+16; iterator one pointer, pair<iterator,bool>8 bytes.
namespace FableTree_005e2580 {
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

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace FableTree_005e2580 {
void _Rb_tree_iterator::_M_decrement(){if(Node->Left){Node=Node->Left;while(Node->Right)Node=Node->Right;}else{_Rb_tree_node* y=Node->Parent;while(Node==y->Left){Node=y;y=y->Parent;}Node=y;}}
_Rb_tree_iterator _Rb_tree::_M_insert(_Rb_tree_node*,_Rb_tree_node* parent,CBasicString<char>*const& value,bool forceLeft){_Rb_tree_node* n=new _Rb_tree_node;n->Color=0;n->Parent=parent;n->Left=n->Right=0;n->Value=value;if(parent==Header){Header->Parent=Header->Left=Header->Right=n;}else if(forceLeft || Compare(value,parent->Value)){parent->Left=n;if(Header->Left==parent)Header->Left=n;}else{parent->Right=n;if(Header->Right==parent)Header->Right=n;}return _Rb_tree_iterator(n);}
void Clear(_Rb_tree_node* n){if(n){Clear(n->Left);Clear(n->Right);delete n;}}
}
int main(){using namespace FableTree_005e2580;_Rb_tree_node header;memset(&header,0,sizeof header);header.Left=header.Right=&header;_Rb_tree tree;tree.Header=&header;
char alpha[]="alpha",beta[]="beta",gamma[]="gamma";CBasicString<char> s[3];s[0].Data=beta;s[1].Data=alpha;s[2].Data=gamma;CBasicString<char>* p=&s[0];
pair<_Rb_tree_iterator,bool> a=tree.insert_unique(p);assert(a.second && a.first.Node==header.Parent);
p=&s[1];pair<_Rb_tree_iterator,bool> b=tree.insert_unique(p);assert(b.second && b.first.Node==header.Left);
p=&s[2];pair<_Rb_tree_iterator,bool> c=tree.insert_unique(p);assert(c.second && c.first.Node==header.Right);
p=&s[0];pair<_Rb_tree_iterator,bool> duplicate=tree.insert_unique(p);assert(!duplicate.second && duplicate.first.Node==a.first.Node);
p=0;pair<_Rb_tree_iterator,bool> empty=tree.insert_unique(p);assert(empty.second && empty.first.Node==header.Left && empty.first.Node->Value==0);pair<_Rb_tree_iterator,bool> same=tree.insert_unique(p);assert(!same.second);
Clear(header.Parent);puts("005e2580_TEST PASS");return 0;}