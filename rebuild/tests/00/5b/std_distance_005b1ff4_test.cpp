// Test for 0x005B1FF4 std::distance over the map<NUISystem::CComponent*, pair<long, long> > iterator (NF42 round 5: was the
// placeholder GetCellBoundingBox/helper_5b1afd test). Links the row's explicit instantiation (declared here without a body) and its
// __distance / _M_increment COMDATs; walks a hand-built three-node tree.
#include <cstdio>
#include "stlport/fable_map.h"
namespace NUISystem { class CComponent; }
typedef std::pair<NUISystem::CComponent* const, std::pair<long, long> > V;
typedef std::_Rb_tree_iterator<V, std::_Nonconst_traits<V> > It;
namespace std { template<class I> ptrdiff_t __cdecl distance(const I& first, const I& last); }
int main()
{
    std::_Rb_tree_node_base h, a, b, c;
    // root b, left a, right c; the header's parent is the root, left the minimum, right the maximum
    h._M_color = false; h._M_parent = &b; h._M_left = &a; h._M_right = &c;
    b._M_color = true;  b._M_parent = &h; b._M_left = &a; b._M_right = &c;
    a._M_color = false; a._M_parent = &b; a._M_left = 0;  a._M_right = 0;
    c._M_color = false; c._M_parent = &b; c._M_left = 0;  c._M_right = 0;
    It first((std::_Rb_tree_node<V>*)&a), mid((std::_Rb_tree_node<V>*)&b), last((std::_Rb_tree_node<V>*)&h);
    ptrdiff_t n3 = std::distance<It>(first, last);
    ptrdiff_t n2 = std::distance<It>(mid, last);
    ptrdiff_t n0 = std::distance<It>(last, last);
    if (n3 == 3 && n2 == 2 && n0 == 0)
        printf("005b1ff4_TEST PASS\n");
    else
        printf("FAIL n3=%d n2=%d n0=%d\n", (int)n3, (int)n2, (int)n0);
    return 0;
}
