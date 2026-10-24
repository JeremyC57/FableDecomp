#include <cstdio>
#include <cstring>

// Model: an STLport map<CDefString, long> (header node, count) walked in order; CDefString == CCharString by text.
class CCharString {
public:
    const char* Text;
};

class CDefString {
public:
    const char* Name;
    bool operator==(const CCharString& other) const { return std::strcmp(Name, other.Text) == 0; }
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;
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

template <class K, class V> struct map {
    typedef pair<K, V> value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;
    };
    struct const_iterator {
        _Rb_tree_node_base* _M_node;
        const_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        const value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
        const_iterator& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    unsigned int        _M_key_compare;
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};

class CNamedValueTreeOwner {
public:
    unsigned char         _pad00[0x40];
    map<CDefString, long> m_attributes;

    bool FindAttributeByName(const CCharString& name, long* value) const
    {
        for (map<CDefString, long>::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
            if ((*it).first == name) {
                *value = (*it).second;
                return true;
            }
        }
        return false;
    }
};

typedef map<CDefString, long>::_Node Node;

static void Set(Node& n, const char* name, long v, _Rb_tree_node_base* parent, _Rb_tree_node_base* l, _Rb_tree_node_base* r)
{
    n._M_value_field.first.Name = name;
    n._M_value_field.second = v;
    n._M_parent = parent;
    n._M_left = l;
    n._M_right = r;
}

int main()
{
    // tree:  header <-> root "m"(20); "m".left = "d"(10) with right child "g"(15); "m".right = "t"(30)
    _Rb_tree_node_base header;
    Node m, d, g, t;
    Set(m, "m", 20, &header, &d, &t);
    Set(d, "d", 10, &m, 0, &g);
    Set(g, "g", 15, &d, 0, 0);
    Set(t, "t", 30, &m, 0, 0);
    header._M_parent = &m;
    header._M_left = &d;
    header._M_right = &t;
    CNamedValueTreeOwner owner;
    owner.m_attributes._M_header = &header;
    owner.m_attributes._M_node_count = 4;

    const char* names[4] = { "d", "g", "m", "t" };
    long values[4] = { 10, 15, 20, 30 };
    for (int i = 0; i < 4; ++i) {
        CCharString key = { names[i] };
        long out = -1;
        if (!owner.FindAttributeByName(key, &out) || out != values[i]) {
            std::printf("007817f0_TEST FAIL\n");
            return 1;
        }
    }
    CCharString missing = { "zz" };
    long out = 77;
    if (owner.FindAttributeByName(missing, &out) || out != 77) {
        std::printf("007817f0_TEST FAIL\n");
        return 1;
    }
    // empty map: begin() == end()
    _Rb_tree_node_base empty;
    empty._M_left = &empty;
    empty._M_right = &empty;
    empty._M_parent = 0;
    owner.m_attributes._M_header = &empty;
    CCharString key = { "m" };
    if (owner.FindAttributeByName(key, &out)) {
        std::printf("007817f0_TEST FAIL\n");
        return 1;
    }
    std::printf("007817f0_TEST PASS\n");
    return 0;
}