// 009f03b0 CCodeSectionManager::RemoveSection
// Flags: /O2 /Oy /W3 
// Leaf: RemoveSection
#include <stdlib.h>
namespace CodeSections {
// Layout: CCharString4 bytes, map12 bytes, tree node string+10/value+14; section name+4 and active+8.
struct CCharString{void*Data;CCharString();CCharString(const CCharString&);~CCharString();};
struct _Rb_tree_node {unsigned color;_Rb_tree_node*parent,*left,*right;CCharString name;unsigned value;};
struct _Rb_index_node {unsigned color;_Rb_index_node*parent,*left,*right;unsigned key,value;};
struct _Rb_tree_iterator{_Rb_tree_node*node;_Rb_tree_iterator(){} __forceinline _Rb_tree_iterator(const _Rb_tree_iterator&v):node(v.node){}};
_Rb_tree_node* __cdecl _Rb_tree_rebalance_for_erase(_Rb_tree_node*,_Rb_tree_node*&, _Rb_tree_node*&, _Rb_tree_node*&);
struct CStringSectionMap{_Rb_tree_node*header;unsigned count,pad;_Rb_tree_node*find(const CCharString&);void erase(_Rb_tree_iterator);};
struct CSectionIndexMap{_Rb_tree_node*header;unsigned count,pad;_Rb_tree_iterator find(const unsigned&);void erase(_Rb_tree_iterator);};
struct CCodeSection {unsigned index;CCharString name;bool active;};
struct CCodeSectionManager {void*vtable;CStringSectionMap Names;CStringSectionMap References;CSectionIndexMap Indices;
 CCodeSection*GetSection(unsigned);void RemoveSection(CCharString);
};
__declspec(noinline) _Rb_tree_iterator CSectionIndexMap::find(const unsigned&key){
 _Rb_index_node*x=(_Rb_index_node*)header->parent;_Rb_index_node*y=(_Rb_index_node*)header;
 while(x){if(x->key>=key){y=x;x=x->left;}else x=x->right;}
 if(y==(_Rb_index_node*)header||key<y->key)y=(_Rb_index_node*)header;_Rb_tree_iterator out;out.node=(_Rb_tree_node*)y;return out;
}
void CCodeSectionManager::RemoveSection(CCharString name){
 CStringSectionMap&names=Names;
 _Rb_tree_node*node=names.find(name);
 if(node!=names.header){unsigned index=node->value;
  if(GetSection(index)->active && index!=~0u && GetSection(index)->active)GetSection(index)->active=false;
  _Rb_tree_node*removed=_Rb_tree_rebalance_for_erase(node,names.header->parent,names.header->left,names.header->right);
  removed->name.~CCharString();if(removed)free(removed);--names.count;
  if(index>0){CCharString copy(GetSection(index)->name);_Rb_tree_node*p=References.find(copy);if(p!=References.header){_Rb_tree_iterator it;it.node=p;References.erase(it);}_Rb_tree_iterator it=Indices.find(index);if(it.node!=Indices.header)Indices.erase(it);}
 }
}
}

#include <stdio.h>
#include <assert.h>
#include <new>
namespace CodeSections {
struct StringBody{unsigned refs,id;};static int Bodies,Copies,Destructions,NameErases,RefErases,IndexErases,Lookups;
CCharString::CCharString(){StringBody*b=new StringBody;b->refs=1;b->id=7;Data=b;++Bodies;}
CCharString::CCharString(const CCharString&v):Data(v.Data){++((StringBody*)Data)->refs;++Copies;}
CCharString::~CCharString(){StringBody*b=(StringBody*)Data;assert(b&&b->refs);if(!--b->refs){delete b;--Bodies;}++Destructions;}
static CCodeSection Sections[2],Sentinel;
_Rb_tree_node*CStringSectionMap::find(const CCharString&v){return header->parent&&((StringBody*)header->parent->name.Data)->id==((StringBody*)v.Data)->id?header->parent:header;}
void CStringSectionMap::erase(_Rb_tree_iterator i){assert(i.node==header->parent);++RefErases;header->parent=0;header->left=header->right=header;--count;i.node->~_Rb_tree_node();free(i.node);}
void CSectionIndexMap::erase(_Rb_tree_iterator i){assert(i.node==header->parent);++IndexErases;header->parent=0;header->left=header->right=header;--count;free(i.node);}
_Rb_tree_node* __cdecl _Rb_tree_rebalance_for_erase(_Rb_tree_node*n,_Rb_tree_node*&root,_Rb_tree_node*&left,_Rb_tree_node*&right){assert(root==n);++NameErases;root=0;left=right=n->parent;return n;}
CCodeSection*CCodeSectionManager::GetSection(unsigned n){++Lookups;assert(n==0||n==1||n==~0u);return n==~0u?&Sentinel:Sections+n;}
static void Header(_Rb_tree_node&h){h.parent=0;h.left=h.right=&h;}
static _Rb_tree_node*Node(_Rb_tree_node&h,unsigned index){_Rb_tree_node*n=new(malloc(sizeof(_Rb_tree_node)))_Rb_tree_node;n->parent=&h;n->left=n->right=0;n->value=index;h.parent=h.left=h.right=n;return n;}
static void Case(unsigned index,bool present,bool active,bool refPresent,bool indexPresent){
 _Rb_tree_node h1,h2,h3;Header(h1);Header(h2);Header(h3);CCodeSectionManager m;m.Names.header=&h1;m.References.header=&h2;m.Indices.header=&h3;m.Names.count=m.References.count=m.Indices.count=0;
 CCodeSection*section=index==~0u?&Sentinel:Sections+index;section->active=active;CCharString name;
 if(present){Node(h1,index);m.Names.count=1;}if(refPresent){Node(h2,index);m.References.count=1;}
 if(indexPresent){_Rb_index_node*n=new(malloc(sizeof(_Rb_index_node)))_Rb_index_node;n->parent=(_Rb_index_node*)&h3;n->left=n->right=0;n->key=index;h3.parent=(_Rb_tree_node*)n;m.Indices.count=1;}
 int ne=NameErases,re=RefErases,ie=IndexErases,bc=Bodies,cp=Copies,dd=Destructions;Lookups=0;m.RemoveSection(name);
 assert(NameErases-ne==int(present));assert(RefErases-re==int(present&&index>0&&refPresent));assert(IndexErases-ie==int(present&&index>0&&indexPresent));assert(m.Names.count==0);
 assert(section->active==(active&&(!present||index==~0u)));assert(Copies-cp==(present&&index>0?2:1));assert(Destructions-dd==(present?2:1)+(present&&index>0?1:0)+(present&&index>0&&refPresent?1:0));assert(Bodies==bc-int(present)-int(present&&index>0&&refPresent));
 if(!present)assert(Lookups==0);
 if(h2.parent){_Rb_tree_iterator it;it.node=h2.parent;m.References.erase(it);}if(h3.parent){_Rb_tree_iterator it;it.node=h3.parent;m.Indices.erase(it);}
}
}
int main(){using namespace CodeSections;int baseline=Bodies;Case(1,false,false,false,false);Case(0,true,true,false,false);Case(1,true,true,true,true);Case(1,true,false,false,false);Case(1,true,false,true,false);Case(1,true,false,false,true);Case(~0u,true,true,true,true);assert(Bodies==baseline);puts("009f03b0_TEST PASS");}