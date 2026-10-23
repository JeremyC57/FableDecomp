// 009efb80 CCacheInfo_InsertOrFind
// Flags: /O2 /Oy /W3 
// Leaf: insert_unique
// Layout: header pointer at+0; RB node color,parent,left,right,key at0,4,8,c,10. CCharString is one pointer to CBasicString<char>.
namespace CacheTree {
template<class C>struct CBasicString{C*data;bool operator<(const CBasicString&)const;};
template<> __declspec(noinline) bool CBasicString<char>::operator<(const CBasicString&v)const {const char*a=data;const char*b=v.data;while(*a||*b){if(*a<*b)return true;if(*a>*b)return false;++a;++b;}return false;}
struct CCharString{CBasicString<char>*p;__forceinline bool operator<(const CCharString&v)const{bool result;if(v.p==p)result=false;else if(!p)result=true;else if(!v.p)result=false;else result=*p<*v.p;return result;}};
namespace _STL {
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

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace CacheTree {

namespace _STL {
void _Rb_tree_iterator::_M_decrement(){if(node->left){node=node->left;while(node->right)node=node->right;}else{_Rb_tree_node*p=node->parent;while(node==p->left){node=p;p=p->parent;}node=p;}}
_Rb_tree_iterator _Rb_tree::_M_insert(_Rb_tree_node*,_Rb_tree_node*y,const CCharString&v,bool){_Rb_tree_node*n=new _Rb_tree_node;memset(n,0,sizeof(*n));n->parent=y;n->key=v;if(y==header){header->parent=n;header->left=n;header->right=n;}else if(v<y->key){y->left=n;if(header->left==y)header->left=n;}else{y->right=n;if(header->right==y)header->right=n;}++count;_Rb_tree_iterator r;r.node=n;return r;}
}
}
int main(){using namespace CacheTree;_STL::_Rb_tree t;_STL::_Rb_tree_node h;memset(&h,0,sizeof(h));t.header=&h;t.count=0;h.left=h.right=&h;
 CBasicString<char> sb,se,sc,scc,sa;sb.data=(char*)"b";se.data=(char*)"e";sc.data=(char*)"c";scc.data=(char*)"c";sa.data=(char*)"a";
 CCharString b,e,c,cc,a,empty;b.p=&sb;e.p=&se;c.p=&sc;cc.p=&scc;a.p=&sa;empty.p=0;
 _STL::pair pb=t.insert_unique(b);assert(pb.second&&t.count==1&&pb.first.node==h.parent);
 _STL::pair pe=t.insert_unique(e);assert(pe.second&&t.count==2&&pe.first.node==h.right);
 _STL::pair pc=t.insert_unique(c);assert(pc.second&&t.count==3&&pc.first.node==pe.first.node->left);
 _STL::pair duplicate=t.insert_unique(cc);assert(!duplicate.second&&duplicate.first.node==pc.first.node&&t.count==3);
 _STL::pair pa=t.insert_unique(a);assert(pa.second&&t.count==4&&pa.first.node==h.left);
 _STL::pair pn=t.insert_unique(empty);assert(pn.second&&t.count==5&&pn.first.node==h.left);
 _STL::pair duplicateNull=t.insert_unique(empty);assert(!duplicateNull.second&&duplicateNull.first.node==pn.first.node&&t.count==5);
 delete pn.first.node;delete pa.first.node;delete pc.first.node;delete pe.first.node;delete pb.first.node;puts("009efb80_TEST PASS");return 0;}