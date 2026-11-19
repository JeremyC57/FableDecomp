// Behaviour test for 0x005698A0 std::map<long, CCountedPointer<CTexture> >::operator[]: the row verbatim, driven through a real STLport
// map. Missing ids (including a negative one: signed compare) are inserted with an empty pointer; existing ids return the same element.
#include <stdio.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/CTCHeroOnlineScoreboard.h"

namespace std {
template CCountedPointer<CTexture>& map<long, CCountedPointer<CTexture> >::operator[](const long&);
}
// Test stand-in for STLport's out-of-line _Rb_global<bool>::_Rebalance (retail library code, 0x0042971D): no rotations, the tree stays a
// valid (unbalanced) binary search tree, which is all operator[]'s lower_bound walk needs.
namespace std {
template<> void __cdecl _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*& root) { root->_M_color = true; }
}
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }

int main()
{
    std::map<long, CCountedPointer<CTexture> > cache;
    CCountedPointer<CTexture>& a = cache[5];
    check(cache.size() == 1 && a.Data == 0 && a.Info == 0, "inserted empty");
    CCountedPointer<CTexture>& b = cache[-3];
    CCountedPointer<CTexture>& c = cache[9];
    check(cache.size() == 3 && &a != &b && &b != &c, "three ids");
    check(&cache[5] == &a && &cache[-3] == &b && &cache[9] == &c && cache.size() == 3, "found again, nothing inserted");
    check(cache.begin()->first == -3, "negative id sorts first (signed)");
    if (!g_fail) printf("005698a0_TEST PASS\n");
    return g_fail;
}