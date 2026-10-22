// Behaviour test for 00619950 CLongToVectorMap::_M_clone_node (malloc node, copy value, copy colour, null children)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct CRetailVector12 {
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CRetailVector12() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CRetailVector12(const CRetailVector12& other) : _M_start(other._M_start), _M_finish(other._M_finish), _M_end_of_storage(other._M_end_of_storage) { ++g_copies; }
};
struct SLongVectorPair {
    long first; CRetailVector12 second;
    SLongVectorPair() : first(0) {}
    SLongVectorPair(const SLongVectorPair& other) : first(other.first), second(other.second) {}
};
struct SRbNodeBase { unsigned char _M_color; SRbNodeBase* _M_parent; SRbNodeBase* _M_left; SRbNodeBase* _M_right; };
struct SRbNode : SRbNodeBase { SLongVectorPair _M_value_field; };
struct CLongToVectorMap {
    SRbNodeBase* _M_header; unsigned long _M_node_count;
    SRbNode* _M_create_node(const SLongVectorPair& v)
    {
        SRbNode* tmp = (SRbNode*)std::malloc(sizeof(SRbNode));
        new (&tmp->_M_value_field) SLongVectorPair(v);
        return tmp;
    }
    SRbNode* _M_clone_node(SRbNode* x);
};
SRbNode* CLongToVectorMap::_M_clone_node(SRbNode* x)
{
    SRbNode* tmp = _M_create_node(x->_M_value_field);
    tmp->_M_color = x->_M_color;
    tmp->_M_left = 0;
    tmp->_M_right = 0;
    return tmp;
}
int main()
{
    int marker;
    SRbNode src; src._M_color = 1; src._M_parent = &src; src._M_left = &src; src._M_right = &src;
    src._M_value_field.first = 77; src._M_value_field.second._M_start = &marker;
    CLongToVectorMap m; m._M_header = 0; m._M_node_count = 0;
    SRbNode* c = m._M_clone_node(&src);
    if (!c || c == &src || c->_M_color != 1 || c->_M_left || c->_M_right || c->_M_value_field.first != 77
        || c->_M_value_field.second._M_start != &marker || g_copies != 1) { std::printf("00619950_TEST FAIL\n"); return 1; }
    std::free(c);
    std::printf("00619950_TEST PASS\n");
    return 0;
}