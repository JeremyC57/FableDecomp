#include <cstdio>
namespace NUISystem { class CManager { public: enum EUsedKeys { USED_KEY_NONE }; }; }
namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;
    iterator insert(iterator position, const value_type& v);
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
template long& map<NUISystem::CManager::EUsedKeys, long>::operator[](const NUISystem::CManager::EUsedKeys&);
}

#include <cstdlib>
typedef NUISystem::CManager::EUsedKeys Key; typedef long Val;
typedef std::map<Key, Val> Map;
typedef std::_Rb_tree_node<Map::value_type> Node;
static Node* g_node = 0; static int g_inserts = 0; static Key g_inserted_key;
template <class K, class T, class C> std::_Rb_tree_node_base* std::map<K, T, C>::_M_lower_bound(const K& k) const
{ return (g_node && !(((Node*)g_node)->_M_value_field.first < k)) ? (std::_Rb_tree_node_base*)g_node : _M_header; }
template <class K, class T, class C> typename std::map<K, T, C>::iterator std::map<K, T, C>::insert(iterator, const value_type& v)
{ ++g_inserts; g_inserted_key = v.first; Node* n = (Node*)std::calloc(1, sizeof(Node)); *(Key*)&n->_M_value_field.first = v.first; n->_M_value_field.second = v.second; g_node = n; return iterator(n); }
int main()
{
    static std::_Rb_tree_node_base header; Map m; m._M_header = &header;
    Key k = (NUISystem::CManager::EUsedKeys)5;
    Val& v = m[k];
    bool ok = g_inserts == 1 && g_inserted_key == k && v == (Val)0 && &v == &g_node->_M_value_field.second;
    v = (Val)42;
    ok = ok && m[k] == (Val)42 && g_inserts == 1;
    Key lo = (NUISystem::CManager::EUsedKeys)3; m[lo];
    ok = ok && g_inserts == 2;
    if (ok) std::printf("0042d201_TEST PASS\n");
    return 0;
}