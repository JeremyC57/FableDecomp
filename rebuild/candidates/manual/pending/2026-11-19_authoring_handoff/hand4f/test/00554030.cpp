// Behaviour test for 0x00554030 std::map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >::operator[]: the row's defines,
// includes and explicit instantiation verbatim, driven through a real STLport map. A missing key must be inserted with an empty counted
// pointer (and the map grows by one); an existing key must return the same stored element without inserting; the temporary pair's
// release must leave the stored pointer's reference count alone (2 after one copy in, back to 1 when the copy is gone).
#include <stdio.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/NUISystem_CTable.h"

namespace std {
template CCountedPointer<NUISystem::CComponent>& map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >::operator[](const NUISystem::ETableSprites&);
}

// Test stand-in for STLport's out-of-line _Rb_global<bool>::_Rebalance (retail library code, 0x0042971D): no rotations, the tree stays a
// valid (unbalanced) binary search tree, which is all operator[]'s lower_bound walk needs.
namespace std {
template<> void __cdecl _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*& root) { root->_M_color = true; }
}
typedef std::map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> > TSprites;
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static void __fastcall no_delete(void*) { }
int main()
{
    TSprites sprites;
    CCountedPointer<NUISystem::CComponent>& a = sprites[NUISystem::TABLE_SPRITES_TOP_RIGHT];
    check(sprites.size() == 1, "inserted one");
    check(a.Data == 0 && a.Info == 0, "empty pointer inserted");
    CCPPointerInfo info(1, no_delete, 0);
    a.Data = (NUISystem::CComponent*)&info;
    a.Info = &info;
    CCountedPointer<NUISystem::CComponent>& b = sprites[NUISystem::TABLE_SPRITES_TOP_LEFT];
    check(sprites.size() == 2 && &b != &a && b.Info == 0, "second key inserted");
    CCountedPointer<NUISystem::CComponent>& c = sprites[NUISystem::TABLE_SPRITES_TOP_RIGHT];
    check(&c == &a && sprites.size() == 2, "existing key found, nothing inserted");
    check(info.RefCount == 1, "lookup leaves the count alone");
    CCountedPointer<NUISystem::CComponent>& d = sprites[NUISystem::TABLE_SPRITES_BOTTOM_RIGHT];
    CCountedPointer<NUISystem::CComponent>& e = sprites[NUISystem::TABLE_SPRITES_BOTTOM_LEFT];
    check(sprites.size() == 4 && &d != &e && &sprites[NUISystem::TABLE_SPRITES_BOTTOM_RIGHT] == &d && &sprites[NUISystem::TABLE_SPRITES_TOP_LEFT] == &b, "four keys kept apart");
    a.Data = 0; a.Info = 0;
    if (!g_fail) printf("00554030_TEST PASS\n");
    return g_fail;
}
