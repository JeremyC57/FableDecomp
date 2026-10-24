// CNamedValueTreeOwner::FindAttributeByName(const CCharString& name, long* value) -- walk the owner's def-string -> value map in order and return the value of the first entry whose name equals `name`
// Leaf: FindAttributeByName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007817F0, 118 bytes, __fastcall `ret 8`.
// Caller: 0x007808B0 (catalogued CTCAvoidRegion_Initialize) calls it on the object at its +0x0c, the same owner the landed
//   0x00781B90 (catalogued CTCAvoidRegion_BuildVectorFromTree) walks; no CTCAvoidRegion class exists in the PDBs and no
//   PDB class holds a map<CDefString, long> at +0x40, so the owner keeps a descriptive name.
// Callees: CDefString::operator==(const CCharString&) const @ 0x004B7980.
// Layout: an STLport map<CDefString, long> at +0x40 (retail 12 bytes: the header-node pointer, the node count and the
//   empty comparator); node = color +0, parent +4, left +8, right +0x0c, pair<const CDefString, long> +0x10 / +0x14.
// Shape: a linear `for (it = begin(); it != end(); ++it)` scan (the map is keyed by the def string's CRC, so a name is
//   matched entry by entry): `name` is loaded once into ebx, end() (the header pointer) is re-read from the member after
//   every call, `++it` inlines STLport's `_Rb_global<bool>::_M_increment`; the hit stores `it->second` through the out
//   pointer and returns true, the miss returns false.
class CCharString;

class CDefString {
public:
    unsigned long CRC;
    bool operator==(const CCharString& other) const;     // 0x004B7980
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

template <class K, class V> struct map {                 // STLport map, 12 bytes in retail
    typedef pair<const K, V> value_type;
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
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned long       _M_node_count;                   // +0x04
    unsigned long       _M_key_compare;                  // +0x08 (empty comparator, padded)
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};

class CNamedValueTreeOwner {
public:
    unsigned char           _pad00[0x40];
    map<CDefString, long>   m_attributes;                // +0x40

    bool FindAttributeByName(const CCharString& name, long* value) const;
};

bool CNamedValueTreeOwner::FindAttributeByName(const CCharString& name, long* value) const
{
    for (map<CDefString, long>::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it) {
        if ((*it).first == name) {
            *value = (*it).second;
            return true;
        }
    }
    return false;
}