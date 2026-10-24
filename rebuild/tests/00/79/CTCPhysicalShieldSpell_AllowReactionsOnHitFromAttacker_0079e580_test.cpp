#include <cstdio>
#include <cstdlib>

// Model: a sorted-set stand-in (insert_unique/find/_Rebalance_for_erase over a sorted array of nodes); allow=true adds the
// attacker once, allow=false removes it when present (freeing its node, dropping the count) and ignores absent ones.
class CThing { public: int Id; };

static int g_frees = 0;
static void FreeNode(void* p) { ++g_frees; std::free(p); }

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> inline void _Destroy(T*) { }

struct CThingSet {
    typedef CThing* value_type;
    struct _Node : _Rb_tree_node_base { value_type _M_value_field; };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator() { }
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    _Node*              Nodes[16];                       // stand-in storage (the tree shape is not exercised here)

    pair<iterator, bool> insert_unique(const value_type& v)
    {
        for (unsigned long i = 0; i < _M_node_count; ++i)
            if (Nodes[i]->_M_value_field == v)
                return pair<iterator, bool>(iterator(Nodes[i]), false);
        _Node* n = (_Node*)std::malloc(sizeof(_Node));
        n->_M_value_field = v;
        Nodes[_M_node_count++] = n;
        return pair<iterator, bool>(iterator(n), true);
    }
    iterator find(const value_type& k)
    {
        for (unsigned long i = 0; i < _M_node_count; ++i)
            if (Nodes[i]->_M_value_field == k)
                return iterator(Nodes[i]);
        return iterator(_M_header);
    }
    static _Rb_tree_node_base* Rebalance(CThingSet* set, _Rb_tree_node_base* z)
    {
        unsigned long i = 0;
        while (Nodes_at(set, i) != z)
            ++i;
        for (; i + 1 < set->_M_node_count; ++i)
            set->Nodes[i] = set->Nodes[i + 1];
        return z;
    }
    static _Node* Nodes_at(CThingSet* set, unsigned long i) { return set->Nodes[i]; }
    iterator end() { return iterator(_M_header); }
    pair<iterator, bool> insert(const value_type& v) { return insert_unique(v); }
    void erase(iterator position)
    {
        _Node* y = static_cast<_Node*>(Rebalance(this, position._M_node));
        _Destroy(&y->_M_value_field);
        if (y)
            FreeNode(y);
        --_M_node_count;
    }
};

class CTCPhysicalShieldSpell {
public:
    CThingSet AllowReactionAttackers;
    void AllowReactionsOnHitFromAttacker(CThing* attacker, bool allow);
};

void CTCPhysicalShieldSpell::AllowReactionsOnHitFromAttacker(CThing* attacker, bool allow)
{
    if (allow)
        AllowReactionAttackers.insert(attacker);
    else {
        CThingSet::iterator it = AllowReactionAttackers.find(attacker);
        if (it != AllowReactionAttackers.end())
            AllowReactionAttackers.erase(it);
    }
}

int main()
{
    _Rb_tree_node_base header;
    CTCPhysicalShieldSpell shield;
    shield.AllowReactionAttackers._M_header = &header;
    shield.AllowReactionAttackers._M_node_count = 0;
    CThing a, b, c;
    shield.AllowReactionsOnHitFromAttacker(&a, true);
    shield.AllowReactionsOnHitFromAttacker(&b, true);
    shield.AllowReactionsOnHitFromAttacker(&a, true);          // already present
    bool ok = shield.AllowReactionAttackers._M_node_count == 2;
    shield.AllowReactionsOnHitFromAttacker(&c, false);         // absent: nothing happens
    ok = ok && shield.AllowReactionAttackers._M_node_count == 2 && g_frees == 0;
    shield.AllowReactionsOnHitFromAttacker(&a, false);
    ok = ok && shield.AllowReactionAttackers._M_node_count == 1 && g_frees == 1
         && shield.AllowReactionAttackers.find(&a)._M_node == &header
         && shield.AllowReactionAttackers.find(&b)._M_node != &header;
    if (!ok) {
        std::printf("0079e580_TEST FAIL\n");
        return 1;
    }
    std::printf("0079e580_TEST PASS\n");
    return 0;
}