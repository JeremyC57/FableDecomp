#include <cstdio>

// Model: CTCBoastingCrowdControl's STLport map<unsigned __int64, CIntelligentPointer<CThing>> and crowd things.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

class CThing {
public:
    unsigned int  m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Flags91;
    int           Kills;
    bool          LastKillFlag;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool removeFromWorld) { ++Kills; LastKillFlag = removeFromWorld; }
    template <class T> bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1U << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};

template <class K, class V> struct pair {
    K first;
    V second;
};

static int g_erasedNodes = 0;

struct CHeroCrowdMap {
    typedef pair<unsigned int, CIntelligentPointer<CThing> > value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        iterator& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;

    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void _M_erase(_Rb_tree_node_base* x)
    {
        while (x != 0) {
            _M_erase(x->_M_right);
            _Rb_tree_node_base* y = x->_M_left;
            delete static_cast<_Node*>(x);
            ++g_erasedNodes;
            x = y;
        }
    }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};

#define IS_THING_ALIVE(t) ((t) && !(t)->IsDead())

// three nodes: root (key 2) with children key 1 and key 3; thing per node
static CHeroCrowdMap::_Node* MakeNode(unsigned int key, CThing* thing, _Rb_tree_node_base* parent)
{
    CHeroCrowdMap::_Node* n = new CHeroCrowdMap::_Node;
    n->_M_color = 0;
    n->_M_parent = parent;
    n->_M_left = 0;
    n->_M_right = 0;
    n->_M_value_field.first = key;
    n->_M_value_field.second.m_pItem = thing;
    return n;
}

static void Build(CHeroCrowdMap& map, _Rb_tree_node_base& header, CThing* a, CThing* b, CThing* c)
{
    CHeroCrowdMap::_Node* root = MakeNode(2, b, &header);
    root->_M_left = MakeNode(1, a, root);
    root->_M_right = MakeNode(3, c, root);
    header._M_parent = root;
    header._M_left = root->_M_left;
    header._M_right = root->_M_right;
    map._M_header = &header;
    map._M_node_count = 3;
}

static void InitThing(CThing& t, bool dead)
{
    for (int i = 0; i < 9; ++i)
        t.m_presentBits[i] = 0;
    t.m_interfaces.first = 0;
    t.m_interfaces.last = 0;
    t.Flags91 = dead ? 1 : 0;
    t.Kills = 0;
    t.LastKillFlag = false;
}

class CTCCreatureGroupBoastingCrowd {
public:
    int Notified;
    void OnNoBoastsRemain() { ++Notified; }
};

class CTCBoastingCrowdControl {
public:
    bool          Active;
    CHeroCrowdMap HeroCrowdThings;

    void OnNoBoastsRemain()
    {
        CHeroCrowdMap::iterator end = HeroCrowdThings.end();
        for (CHeroCrowdMap::iterator it = HeroCrowdThings.begin(); it != end; ++it) {
            CThing* crowdThing = (*it).second.GetPItem();
            if (IS_THING_ALIVE(crowdThing)) {
                CTCCreatureGroupBoastingCrowd* crowd;
                crowdThing->GetInterface(&crowd, 0x74);
                crowd->OnNoBoastsRemain();
            }
        }
    }
};

static void GiveCrowd(CThing& t, CInterfaceMap::SEntry* entries, CTCCreatureGroupBoastingCrowd* crowd)
{
    entries[0].type = 0x10;
    entries[0].component = 0;
    entries[1].type = 0x74;
    entries[1].component = (CTCBase*)crowd;
    entries[2].type = 0x90;
    entries[2].component = 0;
    t.m_interfaces.first = entries;
    t.m_interfaces.last = entries + 3;
    t.m_presentBits[0x74 >> 5] |= 1U << (0x74 & 31);
}

int main()
{
    CThing a, b, c;
    InitThing(a, false);
    InitThing(b, true);                                  // dead crowd thing: skipped
    InitThing(c, false);
    CTCCreatureGroupBoastingCrowd ca, cb, cc;
    ca.Notified = cb.Notified = cc.Notified = 0;
    CInterfaceMap::SEntry ea[3], eb[3], ec[3];
    GiveCrowd(a, ea, &ca);
    GiveCrowd(b, eb, &cb);
    GiveCrowd(c, ec, &cc);
    _Rb_tree_node_base header;
    CTCBoastingCrowdControl control;
    Build(control.HeroCrowdThings, header, &a, &b, &c);
    control.OnNoBoastsRemain();
    if (ca.Notified != 1 || cb.Notified != 0 || cc.Notified != 1 || control.HeroCrowdThings._M_node_count != 3) {
        std::printf("00783680_TEST FAIL\n");
        return 1;
    }
    std::printf("00783680_TEST PASS\n");
    return 0;
}