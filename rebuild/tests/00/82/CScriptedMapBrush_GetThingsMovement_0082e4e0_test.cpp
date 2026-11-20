// Behaviour test for 0x0082E4E0 CScriptedMapBrush::GetThingsMovement (links the REAL row object; NF175: the old test re-implemented the
// CSurface::GetHeight stand-in). The tree's out-of-line _M_find (0x0082EEB0) is defined here as a recorder that returns a fake node: the row
// must look the id up in ThingsToMove (+0x20) and return the node's value (+0x18, the pair's second).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSETS
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSTOMOVE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "stlport/fable_map.h"
#include "engine/CScriptedMapBrush.h"
#include <stdio.h>
typedef std::pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> TMoveValue;
typedef std::_Rb_tree<unsigned __int64, TMoveValue, std::_Select1st<TMoveValue>, std::less<unsigned __int64>,
                      std::allocator<TMoveValue> > TMoveTree;
static const void* g_tree; static unsigned __int64 g_key; static int g_calls;
static unsigned long g_node[0x30 / 4];
template<> template<> std::_Rb_tree_node<TMoveValue>* TMoveTree::_M_find<unsigned __int64>(const unsigned __int64& key) const
{
    g_tree = this; g_key = key; ++g_calls;
    return (std::_Rb_tree_node<TMoveValue>*)g_node;
}
int main()
{
    unsigned long brush[0x54 / 4];
    const CScriptedMapBrush* b = (const CScriptedMapBrush*)brush;
    const CScriptedMapBrush::CThingPosAndOrientation& m = b->GetThingsMovement(0x123456789ABCDEF0ui64);
    if (g_calls != 1 || g_tree != (const char*)brush + 0x20 || g_key != 0x123456789ABCDEF0ui64) { printf("FAIL call\n"); return 1; }
    if ((const char*)&m != (const char*)g_node + 0x18) { printf("FAIL result\n"); return 1; }
    printf("GETHEIGHT_0082e4e0_OK\n");
    return 0;
}
