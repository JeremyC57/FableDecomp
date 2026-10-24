// CHeroAbilityStatPackage::IsValid(const CTCHeroExperience&) const -- true when the hero's level in every trainable stat named by the package reaches that stat's threshold
// Leaf: ?IsValid@CHeroAbilityStatPackage@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00851B50, 121 bytes, __fastcall, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?IsValid@CHeroAbilityStatPackage@@QBE_NABVCTCHeroExperience@@@Z, followed in both PDBs by
//   CHeroAbilityStatWeight::GetParameter (TLC 0x00851BD0) -- the same order as retail. The manifest label
//   "CAbility_CheckRequirements" is an autoname. Callers 0x005DAB9C, 0x008E70AC (landed CMeleeCombatAbilityDef::IsAbilityValid).
// Callee: CTCHeroExperience::GetTrainableStatLevel(EHeroTrainableStatType) const 0x006D6520.
// Layout: StatThresholds map<EHeroTrainableStatType, long> at +0 (STLport _Rb_tree: header +0, count +4, compare +8; node
//   value at +0x10). The STLport map/iterator model and the inlined _Rb_global<bool>::_M_increment are the landed
//   GFSerialiseMapBinaryOut 0x00756540 ones.
// Shape: the threshold is read into a local before the call (edi); the comparison operand form re-reads it after the call.
enum EHeroTrainableStatType { };

class CTCHeroExperience {
public:
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const;   // 0x006D6520
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less { };
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class B> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) { }
    const V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    const V* operator->() const { return &((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& o) const { return _M_node != o._M_node; }
};
template <class K, class V, class C = less<K> > class map {     // STLport 4.x _Rb_tree: header, count, compare
public:
    typedef pair<const K, V> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    C                   _M_key_compare;                     // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

class CHeroAbilityStatPackage {
public:
    bool IsValid(const CTCHeroExperience& experience) const;
    std::map<EHeroTrainableStatType, long> StatThresholds;           // +0x00
};

bool CHeroAbilityStatPackage::IsValid(const CTCHeroExperience& experience) const
{
    for (std::map<EHeroTrainableStatType, long>::const_iterator it = StatThresholds.begin(); it != StatThresholds.end(); ++it) {
        long threshold = it->second;
        if (experience.GetTrainableStatLevel(it->first) < threshold)
            return false;
    }
    return true;
}