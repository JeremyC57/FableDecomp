#include <cstdio>
#include <cstdlib>

// Model (CTCDivineWrath): STLport map<CThingCreatureBase*, PerCreatureInfo>::operator[] over an unbalanced BST stand-in for
// insert_unique; existing keys return their record, new keys get a default record (zeroed channel bytes), and every
// temporary intelligent pointer is destroyed again.
class CThingCreatureBase { public: int Id; };
class CThing;

static int g_livePointers = 0;
static int g_copies = 0;

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_livePointers; }
    ~CBaseIntelligentPointer() { --g_livePointers; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};

struct PerCreatureInfo {
    CIntelligentPointer<CThingCreatureBase> ActualCreature;
    long                                    FramesToGetHit;
    CIntelligentPointer<CThing>             Effect;
    CEnginePrimitiveThemeChannelDesc        DefaultChannel;
    PerCreatureInfo() { }
    PerCreatureInfo(const PerCreatureInfo& o)
        : FramesToGetHit(o.FramesToGetHit), DefaultChannel(o.DefaultChannel)
    {
        ActualCreature.m_pItem = o.ActualCreature.m_pItem;
        Effect.m_pItem = o.Effect.m_pItem;
        ++g_copies;
    }
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

struct CCreatureMap {
    typedef CThingCreatureBase* key_type;
    typedef pair<CThingCreatureBase* const, PerCreatureInfo> value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;
        _Node(const value_type& v) : _M_value_field(v) { }
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator==(const iterator& o) const { return _M_node == o._M_node; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    less<key_type>      _M_key_compare;

    static const key_type& _S_key(_Rb_tree_node_base* x) { return static_cast<_Node*>(x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const key_type& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const key_type& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    less<key_type> key_comp() const { return _M_key_compare; }
    iterator insert_unique(iterator, const value_type& v)       // stand-in: plain BST insert
    {
        _Node* z = new _Node(v);
        z->_M_left = z->_M_right = 0;
        _Rb_tree_node_base* parent = _M_header;
        _Rb_tree_node_base** link = &_M_header->_M_parent;
        while (*link) {
            parent = *link;
            link = v.first < _S_key(parent) ? &parent->_M_left : &parent->_M_right;
        }
        *link = z;
        z->_M_parent = parent;
        ++_M_node_count;
        return iterator(z);
    }
    iterator insert(iterator position, const value_type& v) { return insert_unique(position, v); }
    PerCreatureInfo& operator[](const key_type& k);
};

PerCreatureInfo& CCreatureMap::operator[](const key_type& k)
{
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first))
        i = insert(i, value_type(k, PerCreatureInfo()));
    return (*i).second;
}

int main()
{
    _Rb_tree_node_base header;
    header._M_parent = 0;
    header._M_left = header._M_right = &header;
    CCreatureMap m;
    m._M_header = &header;
    m._M_node_count = 0;
    CThingCreatureBase c[3];
    int before = g_livePointers;
    PerCreatureInfo& a = m[&c[1]];
    a.FramesToGetHit = 7;
    bool ok = m._M_node_count == 1 && a.DefaultChannel.ChannelIndex0 == 0 && a.DefaultChannel.ChannelBlendFactor == 0;
    PerCreatureInfo& b = m[&c[0]];
    b.FramesToGetHit = 3;
    PerCreatureInfo& d = m[&c[2]];
    d.FramesToGetHit = 9;
    ok = ok && m._M_node_count == 3;
    int copies = g_copies;
    ok = ok && &m[&c[1]] == &a && m[&c[0]].FramesToGetHit == 3 && m[&c[2]].FramesToGetHit == 9;   // found, no insert
    ok = ok && m._M_node_count == 3 && g_copies == copies;
    ok = ok && g_livePointers == before + 3 * 2;          // only the three stored records' pointers remain
    if (!ok) {
        std::printf("007934f0_TEST FAIL\n");
        return 1;
    }
    std::printf("007934f0_TEST PASS\n");
    return 0;
}