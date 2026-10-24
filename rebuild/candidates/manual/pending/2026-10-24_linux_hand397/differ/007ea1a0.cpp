// map<CThing*,float>::operator[](CThing* const& key) -- the distance slot of a thing in CTCAvoidRegionExit::DistMap: lower_bound walk, and a (key, 0.0f) pair inserted at that hint when the key is absent
// Grade: DIFFER(115v115) -- one store scheduled early: the pair's `second = 0.0f` (mov [esp+18h],0) lands between the hint copy's `mov eax,esp` and `mov [eax],edx`; retail stores it after the hidden-result push (mov [esp+1Ch],0). Everything else, including the slot-homed result read, matches.
// Leaf: ??ACDistMap@@QAEAAMABQAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA1A0, 115 bytes (`ret 4`). STLport map<CThing*, float>::operator[] instantiated
//   for CTCAvoidRegionExit::DistMap (0x013BAF00); its one caller is CTCAvoidRegionExit::UpdateMarkers (`DistMap[parent] =
//   dist` @ 0x007E994A). Catalog label `RBTree_LowerBound` names only the first half.
// Callees: the map's insert(iterator hint, const value_type&) @ 0x007E9DE0 (manifest "RBTree_InsertOrFind": hidden
//   iterator result, the hint by value built in its argument slot).
// Layout: STLport _Rb_tree header-node pointer at +0, node parent +4, left +8, right +0xc, key +0x10, value +0x14.
// Shape: STLport's operator[] verbatim -- `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first))
//   i = insert(i, value_type(k, float())); return (*i).second;` with the iterator's user copy constructor; retail homes
//   the returned iterator in the dead key-argument slot.
class CThing;

struct SRbNodeBase {
    char         _M_color;
    SRbNodeBase* _M_parent;                                     // +0x04
    SRbNodeBase* _M_left;                                       // +0x08
    SRbNodeBase* _M_right;                                      // +0x0c
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
typedef pair<CThing* const, float> SDistPair;                   // the map's value_type

struct SDistNode : SRbNodeBase {
    SDistPair _M_value_field;                                   // +0x10
};

struct _Rb_tree_base_iterator {
    SRbNodeBase* _M_node;
};
struct CDistMapIterator : _Rb_tree_base_iterator {               // STLport _Rb_tree_iterator: body assignments
    CDistMapIterator() { _M_node = 0; }
    CDistMapIterator(SRbNodeBase* n) { _M_node = n; }
    CDistMapIterator(const CDistMapIterator& other) { _M_node = other._M_node; }
    SDistPair& operator*() const { return ((SDistNode*)_M_node)->_M_value_field; }
    bool operator==(const CDistMapIterator& other) const { return _M_node == other._M_node; }
};

struct CDistTree {                                              // STLport _Rb_tree<CThing*, pair<CThing* const, float>, ...>
    SRbNodeBase*  _M_header;
    unsigned long _M_node_count;
    static CThing* const& _S_key(SRbNodeBase* x) { return ((SDistNode*)x)->_M_value_field.first; }
    CDistMapIterator end() { return CDistMapIterator(_M_header); }
    CDistMapIterator lower_bound(CThing* const& k)
    {
        SRbNodeBase* y = _M_header;
        SRbNodeBase* x = _M_header->_M_parent;
        while (x != 0) {
            if (!(_S_key(x) < k)) {
                y = x;
                x = x->_M_left;
            } else
                x = x->_M_right;
        }
        return CDistMapIterator(y);
    }
    CDistMapIterator insert_unique(CDistMapIterator position, const SDistPair& v);   // 0x007E9DE0
};

struct CDistMap {                                               // STLport map<CThing*, float>: the tree as its one member
    CDistTree _M_t;
    CDistMapIterator end() { return _M_t.end(); }
    CDistMapIterator lower_bound(CThing* const& k) { return _M_t.lower_bound(k); }
    CDistMapIterator insert(CDistMapIterator position, const SDistPair& v) { return _M_t.insert_unique(position, v); }
    float& operator[](CThing* const& k);
};

float& CDistMap::operator[](CThing* const& k)
{
    CDistMapIterator i = lower_bound(k);
    if (i == end() || k < (*i).first)
        i = insert(i, SDistPair(k, float()));
    return (*i).second;
}
