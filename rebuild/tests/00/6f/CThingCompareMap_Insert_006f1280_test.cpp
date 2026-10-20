// behaviour test for CThingCompareMap::Insert (0x006F1280): STLport _M_insert linking left/right of y, header root/leftmost/rightmost upkeep, rebalance call, count
#include <cstdio>
#include <cstdlib>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#define K64(h, l) ((u64)(h) << 32 | (u64)(l))
#else
typedef unsigned long long u64;
#define K64(h, l) ((u64)(h) << 32 | (u64)(l))
#endif
inline void* operator new(size_t, void* where) { return where; }
class CThing;
struct SThingCompareValue { u64 first; CThing* second;
    SThingCompareValue(u64 k, CThing* t) : first(k), second(t) {}
    SThingCompareValue(const SThingCompareValue& o) : first(o.first), second(o.second) {} };
struct SRbNodeBase { long _M_color; SRbNodeBase* _M_parent; SRbNodeBase* _M_left; SRbNodeBase* _M_right; };
struct SRbNode : SRbNodeBase { SThingCompareValue _M_value_field; };
static int g_rebal = 0; static SRbNodeBase* g_rebalNode = 0; static SRbNodeBase** g_rebalRoot = 0;
static void Rebalance_0042971d(SRbNodeBase* node, SRbNodeBase*& root) { ++g_rebal; g_rebalNode = node; g_rebalRoot = &root; }
struct CThingCompareMap {
    SRbNodeBase* _M_header; unsigned long _M_node_count;
    struct iterator { SRbNodeBase* _M_node; iterator(SRbNodeBase* n) : _M_node(n) {} };
    SRbNode* _M_create_node(const SThingCompareValue& v) { SRbNode* t = (SRbNode*)std::malloc(sizeof(SRbNode)); new (&t->_M_value_field) SThingCompareValue(v); return t; }
    iterator Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SThingCompareValue& v, SRbNodeBase* w_)
    {
        SRbNode* w = (SRbNode*)w_; SRbNode* y = (SRbNode*)y_; SRbNode* x = (SRbNode*)x_; SRbNode* z;
        if (y == _M_header || (w == 0 && (x != 0 || v.first < y->_M_value_field.first))) {
            z = _M_create_node(v); y->_M_left = z;
            if (y == _M_header) { _M_header->_M_parent = z; _M_header->_M_right = z; }
            else if (y == _M_header->_M_left) _M_header->_M_left = z;
        } else {
            z = _M_create_node(v); y->_M_right = z;
            if (y == _M_header->_M_right) _M_header->_M_right = z;
        }
        z->_M_parent = y; z->_M_left = 0; z->_M_right = 0;
        Rebalance_0042971d(z, _M_header->_M_parent);
        ++_M_node_count;
        return iterator(z);
    }
};
int main()
{
    SRbNodeBase header; header._M_parent = 0; header._M_left = &header; header._M_right = &header;
    CThingCompareMap m; m._M_header = &header; m._M_node_count = 0;
    CThing* t1 = (CThing*)&m; CThing* t2 = (CThing*)&header;
    int bad = 0;
    // empty tree: y == header -> becomes root, leftmost and rightmost
    CThingCompareMap::iterator a = m.Insert(0, &header, SThingCompareValue(K64(1, 5), t1), 0);
    SRbNode* n1 = (SRbNode*)a._M_node;
    if (header._M_parent != n1 || header._M_left != n1 || header._M_right != n1 || n1->_M_parent != &header) bad = 1;
    if (n1->_M_value_field.first != K64(1, 5) || n1->_M_value_field.second != t1 || m._M_node_count != 1) bad = 1;
    if (g_rebal != 1 || g_rebalNode != n1 || g_rebalRoot != &header._M_parent) bad = 1;
    // bigger key (high dword decides) under n1 -> right child, new rightmost
    SRbNode* n2 = (SRbNode*)m.Insert(0, n1, SThingCompareValue(K64(2, 0), t2), 0)._M_node;
    if (n1->_M_right != n2 || header._M_right != n2 || header._M_left != n1 || n2->_M_left != 0 || n2->_M_right != 0) bad = 1;
    // smaller key (low dword decides) under n1 -> left child, new leftmost
    SRbNode* n3 = (SRbNode*)m.Insert(0, n1, SThingCompareValue(K64(1, 4), t2), 0)._M_node;
    if (n1->_M_left != n3 || header._M_left != n3 || n3->_M_parent != n1) bad = 1;
    // w != 0 forces right even for a small key; x != 0 forces left even for a big key
    SRbNode* n4 = (SRbNode*)m.Insert(0, n3, SThingCompareValue(K64(0, 1), t1), n3)._M_node;
    if (n3->_M_right != n4 || header._M_left != n3) bad = 1;
    SRbNode* n5 = (SRbNode*)m.Insert(n2, n2, SThingCompareValue(K64(9, 0), t1), 0)._M_node;
    if (n2->_M_left != n5 || header._M_right != n2 || m._M_node_count != 5 || g_rebal != 5) bad = 1;
    std::free(n1); std::free(n2); std::free(n3); std::free(n4); std::free(n5);
    if (bad) { std::printf("006f1280_TEST FAIL\n"); return 1; }
    std::printf("006f1280_TEST PASS\n");
    return 0;
}