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