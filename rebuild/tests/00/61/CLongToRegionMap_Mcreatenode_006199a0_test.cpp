// Behaviour test for 006199a0 CLongToRegionMap::_M_create_node (malloc node, copy-construct the pair)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_copies = 0;
struct CRegion {
    unsigned char _data[0x58];
    CRegion() { std::memset(_data, 0, sizeof _data); }
    CRegion(const CRegion& other) { std::memcpy(_data, other._data, sizeof _data); ++g_copies; }
};
struct SLongRegionPair {
    long first; CRegion second;
    SLongRegionPair() : first(0) {}
    SLongRegionPair(const SLongRegionPair& other) : first(other.first), second(other.second) {}
};
struct SRbNodeBase { unsigned char _M_color; SRbNodeBase* _M_parent; SRbNodeBase* _M_left; SRbNodeBase* _M_right; };
struct SRbNode : SRbNodeBase { SLongRegionPair _M_value_field; };
struct CLongToRegionMap {
    SRbNodeBase* _M_header; unsigned long _M_node_count;
    SRbNode* _M_create_node(const SLongRegionPair& v);
};
SRbNode* CLongToRegionMap::_M_create_node(const SLongRegionPair& v)
{
    SRbNode* tmp = (SRbNode*)std::malloc(sizeof(SRbNode));
    new (&tmp->_M_value_field) SLongRegionPair(v);
    return tmp;
}
int main()
{
    SLongRegionPair p; p.first = 9; p.second._data[0x57] = 0xAB;
    CLongToRegionMap m; m._M_header = 0; m._M_node_count = 0;
    SRbNode* n = m._M_create_node(p);
    if (!n || n->_M_value_field.first != 9 || n->_M_value_field.second._data[0x57] != 0xAB || g_copies != 1) { std::printf("006199a0_TEST FAIL\n"); return 1; }
    std::free(n);
    std::printf("006199a0_TEST PASS\n");
    return 0;
}