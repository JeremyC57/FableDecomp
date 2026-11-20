// Behaviour test for 0x0082E8F0 CScriptedMapBrush::SetThingToDelete (links the REAL row object; NF123: the old test re-implemented the
// CTCLook::IsTurningBody stand-in). The set's out-of-line insert_unique (0x0082F4B0) is defined here as a recorder: the row must call it on
// the set at +0x14 with the id it was given and drop the result.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH
#define FABLE_RETAIL_CSCRIPTEDMAPBRUSH_THINGSETS
#include "stlport/fable_map.h"
#include "engine/CScriptedMapBrush.h"
#include <stdio.h>
typedef std::_Rb_tree<unsigned __int64, unsigned __int64, std::_Identity<unsigned __int64>, std::less<unsigned __int64>,
                      std::allocator<unsigned __int64> > TUIDTree;
static const void* g_tree; static unsigned __int64 g_value; static int g_calls;
template<> std::pair<TUIDTree::iterator, bool> TUIDTree::insert_unique(const unsigned __int64& value)
{
    g_tree = this; g_value = value; ++g_calls;
    return std::pair<TUIDTree::iterator, bool>(TUIDTree::iterator(0), true);
}
int main()
{
    unsigned long brush[0x54 / 4];
    CScriptedMapBrush* b = (CScriptedMapBrush*)brush;
    b->SetThingToDelete(0x123456789ABCDEF0ui64);
    if (g_calls != 1 || g_tree != (const char*)brush + 0x14 || g_value != 0x123456789ABCDEF0ui64) { printf("FAIL\n"); return 1; }
    printf("ISTURNINGBODY_OK\n");
    return 0;
}
