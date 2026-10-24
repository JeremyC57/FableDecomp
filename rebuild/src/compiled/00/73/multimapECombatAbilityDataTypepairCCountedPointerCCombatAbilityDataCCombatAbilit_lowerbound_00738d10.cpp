// multimap<ECombatAbilityDataType, pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> >::lower_bound(const ECombatAbilityDataType& k) -- STLport tree lower_bound on CTCCombat::CombatAbilities: the first node whose key is not less than k (end() when none), returned through the hidden iterator pointer
// Leaf: ?lower_bound@CCombatAbilityMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00738D10, 43 bytes (48 with padding), thiscall `ret 8`.
// Label correction: catalog "BinaryTree_FindLowerBound" -- callers are the four CTCCombat::ResetBlockPercentageChance<T>
//   instantiations (0x007390B6 / 0x007391C6 / 0x007392D6 / 0x007393E6, `ecx` = CTCCombat+0x70 = CombatAbilities).
// Callees: none.
// Layout: STLport _Rb_tree: header node pointer first; node {colour, parent +4, left +8, right +0xc, key +0x10}.
// Shape: STLport `lower_bound(k) { return iterator(_M_lower_bound(k)); }` with _M_lower_bound inlined: the key is read
//   once into edx (`nop`-aligned loop), the candidate in ecx starts at the header; the iterator has a user
//   constructor, so it is returned through the hidden pointer (`[esp+4]`).
struct CCombatAbilityData {                     // Ego_r.pdb, 0x18 bytes
    void*  __vftable;                           // +0x00
    long   PercentageChance;                    // +0x04
    float  SecondsDuration;                     // +0x08
    float  SecondsToWaitBeforeRepeat;           // +0x0c
    void*  Anim;                                // +0x10 CDefString
    long   InterfaceType;                       // +0x14 ECombatAbilityInterfaceType
};
template <class T> struct CCountedPointer {     // 8 bytes: object + count block
    T*    Object;
    long* Count;
    T* operator->() const { return Object; }
};
struct CCombatAbility {                         // Ego_r.pdb, 8 bytes
    long RepeatTimer;                           // +0x00
    long ActiveTimer;                           // +0x04
    void Reset()
    {
        ActiveTimer = -1;
        RepeatTimer = -1;
    }
};
struct _Rb_tree_node_base {
    long                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};
struct SCombatAbilityPair {                     // pair<CCountedPointer<CCombatAbilityData>, CCombatAbility>
    CCountedPointer<CCombatAbilityData> first;  // +0x00
    CCombatAbility                      second; // +0x08
};
struct SCombatAbilityValue {                    // pair<const ECombatAbilityDataType, SCombatAbilityPair>
    long               first;                   // +0x00
    SCombatAbilityPair second;                  // +0x04
};
struct _Rb_tree_node : _Rb_tree_node_base {
    SCombatAbilityValue _M_value_field;         // +0x10
};
struct _Rb_global {                             // STLport 4.6 _Rb_global<bool> (tree.c), inlined
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
        return _M_node;
    }
};
struct _Rb_tree_iterator {                      // STLport 4.x tree iterator: the node pointer
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    SCombatAbilityValue& operator*() const { return ((_Rb_tree_node*)_M_node)->_M_value_field; }
    SCombatAbilityValue* operator->() const { return &(operator*()); }
    void _M_increment() { _M_node = _Rb_global::_M_increment(_M_node); }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct CCombatAbilityMap {                      // STLport multimap<ECombatAbilityDataType, SCombatAbilityPair>: 12 bytes
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned long       _M_node_count;          // +0x04
    unsigned long       _M_key_compare;         // +0x08 (empty less<>, padded)
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    _Rb_tree_node_base* _M_root() const { return _M_header->_M_parent; }
    static const long& _S_key(_Rb_tree_node_base* x) { return ((_Rb_tree_node*)x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const long& k) const
    {
        _Rb_tree_node_base* y = _M_header;      // last node which is not less than k
        _Rb_tree_node_base* x = _M_root();
        while (x != 0)
            if (!(_S_key(x) < k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    _Rb_tree_node_base* _M_upper_bound(const long& k) const
    {
        _Rb_tree_node_base* y = _M_header;      // last node which is greater than k
        _Rb_tree_node_base* x = _M_root();
        while (x != 0)
            if (k < _S_key(x))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const long& k);        // out of line here
    iterator upper_bound(const long& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const long& k) { return pair<iterator, iterator>(lower_bound(k), upper_bound(k)); }
};


CCombatAbilityMap::iterator CCombatAbilityMap::lower_bound(const long& k)
{
    return iterator(_M_lower_bound(k));
}