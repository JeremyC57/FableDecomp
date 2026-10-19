// Behaviour test for 0x00783F00 (STLport 64-bit-key map _M_insert): runs the reconstructed Insert with a real
// node allocator and a recording rebalance stub, checking left/right linkage, header maintenance and the count.
#include <cstdio>
#include <cstdlib>

#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif
static U64 K(unsigned long hi, unsigned long lo) { return ((U64)hi << 32) | lo; }
struct CBaseObject;
struct CIntelligentPointerThing { void* m_vtable; CBaseObject* m_pItem; };
struct SThingMapValue { U64 first; CIntelligentPointerThing second; };
struct SRbNodeBase { long _M_color; SRbNodeBase* _M_parent; SRbNodeBase* _M_left; SRbNodeBase* _M_right; };
struct SRbNode : SRbNodeBase { SThingMapValue _M_value_field; };

static int g_rebalanced = 0;
static SRbNodeBase** g_rootArg = 0;
void _Rb_tree_rebalance(SRbNodeBase*, SRbNodeBase*& root) { ++g_rebalanced; g_rootArg = &root; }

struct CThingMap64 {
    SRbNodeBase*  _M_header;
    unsigned long _M_node_count;
    struct iterator { SRbNodeBase* _M_node; iterator(SRbNodeBase* n) : _M_node(n) { } };
    SRbNode* _M_create_node(const SThingMapValue& v)
    {
        SRbNode* n = (SRbNode*)std::calloc(1, sizeof(SRbNode));
        n->_M_value_field = v; n->_M_left = n->_M_right = (SRbNodeBase*)1;   // garbage the insert must clear
        return n;
    }
    iterator Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SThingMapValue& v, SRbNodeBase* w_);
};

CThingMap64::iterator CThingMap64::Insert(SRbNodeBase* x_, SRbNodeBase* y_, const SThingMapValue& v, SRbNodeBase* w_)
{
    SRbNode* w = (SRbNode*)w_;
    SRbNode* y = (SRbNode*)y_;
    SRbNode* x = (SRbNode*)x_;
    SRbNode* z;
    if (y == _M_header || (w == 0 && (x != 0 || v.first < y->_M_value_field.first))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_header->_M_parent = z;
            _M_header->_M_right = z;
        }
        else if (y == _M_header->_M_left)
            _M_header->_M_left = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_header->_M_right)
            _M_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_tree_rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

int main()
{
    SRbNode header = SRbNode(); header._M_left = header._M_right = &header; header._M_parent = 0;
    CThingMap64 m; m._M_header = &header; m._M_node_count = 0;
    SThingMapValue v; v.first = K(1, 5); v.second.m_vtable = 0; v.second.m_pItem = 0;
    CThingMap64::iterator r1 = m.Insert(0, &header, v, 0);                  // first node: root
    SRbNode* a = (SRbNode*)r1._M_node;
    bool ok = header._M_parent == a && header._M_right == a && header._M_left == a && a->_M_parent == &header &&
              a->_M_left == 0 && a->_M_right == 0 && m._M_node_count == 1 && a->_M_value_field.first == K(1, 5);
    // larger key under a (high dword larger): goes right, becomes rightmost
    v.first = K(2, 0);
    SRbNode* b = (SRbNode*)m.Insert(0, a, v, 0)._M_node;
    ok = ok && a->_M_right == b && header._M_right == b && m._M_node_count == 2;
    // smaller key (same high, smaller low) under a: left, leftmost
    v.first = K(1, 1);
    SRbNode* c = (SRbNode*)m.Insert(0, a, v, 0)._M_node;
    ok = ok && a->_M_left == c && header._M_left == c && m._M_node_count == 3;
    // x != 0 forces left even with a larger key
    v.first = K(9, 0);
    SRbNode* d = (SRbNode*)m.Insert((SRbNodeBase*)b, b, v, 0)._M_node;
    ok = ok && b->_M_left == d && header._M_right == b;
    // w != 0 forces right
    v.first = 0;
    SRbNode* e = (SRbNode*)m.Insert(0, c, v, (SRbNodeBase*)c)._M_node;
    ok = ok && c->_M_right == e && e->_M_parent == c && g_rebalanced == 5 && g_rootArg == &header._M_parent && m._M_node_count == 5;
    if (!ok) { std::printf("00783f00_TEST FAIL\n"); return 1; }
    std::printf("00783f00_TEST PASS\n");
    return 0;
}
