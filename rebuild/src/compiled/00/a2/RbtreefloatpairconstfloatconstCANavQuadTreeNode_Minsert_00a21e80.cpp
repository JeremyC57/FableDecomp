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
