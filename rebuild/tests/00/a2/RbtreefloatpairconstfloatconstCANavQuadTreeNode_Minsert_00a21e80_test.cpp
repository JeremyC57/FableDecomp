#define FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY
#define FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
// Leaf: ?_M_insert@?$_Rb_tree@MU?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@U?$_Select1st@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@2@U?$less@M@2@V?$allocator@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@U?$_Nonconst_traits@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBMPBVCANavQuadTreeNode@@@2@0@Z
// Flags: /Gr
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_ITERATOR_TRAITS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
class CANavQuadTreeNode;
typedef std::pair<const float, const CANavQuadTreeNode*> NodeEntry;
typedef std::_Rb_tree<float, NodeEntry, std::_Select1st<NodeEntry>, std::less<float>, std::allocator<NodeEntry> > NodeTree;
template NodeTree::iterator NodeTree::_M_insert(std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const NodeEntry&, std::_Rb_tree_node_base*);
#include <cassert>
#include <cstdio>
namespace std {
template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base* z,_Rb_tree_node_base*&) { z->_M_color=false; }
}
int main(){
    std::multimap<float,const CANavQuadTreeNode*> tree;
    float keys[]={3,1,4,2,4,0};int identifiers[6];
    for(int i=0;i<6;++i)tree.insert(NodeEntry(keys[i],reinterpret_cast<const CANavQuadTreeNode*>(&identifiers[i])));
    int expected[]={5,1,3,0,2,4};int count=0;
    for(std::multimap<float,const CANavQuadTreeNode*>::iterator it=tree.begin();it!=tree.end();++it,++count){
        assert(count<6);assert(it->second==reinterpret_cast<const CANavQuadTreeNode*>(&identifiers[expected[count]]));
    }
    assert(count==6);puts("00a21e80_TEST PASS");return 0;
}
