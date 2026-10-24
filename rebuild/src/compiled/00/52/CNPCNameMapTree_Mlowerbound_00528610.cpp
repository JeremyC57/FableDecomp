// CNPCNameMapTree::_M_lower_bound(const unsigned __int64& k) const -- STLport _Rb_tree::_M_lower_bound of CThingManager::NPCNameMap (map<unsigned __int64, pair<CCharString, CWideString> >): the first node whose key is not below k, the header when none
// Leaf: ?_M_lower_bound
//
// Retail 0x00528610, 55 bytes (+ int3 padding to 64), `ret 4`, node pointer in eax.  No callees (less<unsigned __int64> inline).
// Callers: map::lower_bound 0x00528FF0 (wraps the node in an iterator), CThingManager's NPCNameMap operator[]
//   0x0052B8B0 and 0x00529F50.
// Layout: Ego_r.pdb CThingManager::NPCNameMap +0xF4 = map<unsigned __int64, pair<CCharString, CWideString> >; STLport
//   tree = { _M_header (node pointer) +0, _M_node_count +4 }; node { _M_color +0, _M_parent +4, _M_left +8, _M_right +0xC,
//   value +0x10 } with the 64-bit key at +0x10/+0x14.
// Shape: `y = header; x = root; while (x) if (!(key(x) < k)) y = x, x = left(x); else x = right(x); return y;` -- k's two
//   halves load once into esi/edx; the unsigned 64-bit compare is high (`jb`/`ja`) then low (`jb`).
// Label: catalog "BinaryTree_LowerBound_Uint64" (propagated); this is the STLport lower-bound walk.
class CCharString { public: const char* PString; };
class CWideString { public: const unsigned short* PString; };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

struct _Rb_tree_node_base {
    char                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CNPCNameMapTree {                        // STLport _Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString, CWideString> >, ...>
public:
    typedef std::pair<const unsigned __int64, std::pair<CCharString, CWideString> > value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node*        _M_header;                   // +0x0
    unsigned long _M_node_count;               // +0x4

    static const unsigned __int64& _S_key(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const unsigned __int64& k) const;
};

_Rb_tree_node_base* CNPCNameMapTree::_M_lower_bound(const unsigned __int64& k) const
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_header->_M_parent;
    while (x != 0)
        if (!(_S_key(x) < k))
            y = x, x = x->_M_left;
        else
            x = x->_M_right;
    return y;
}