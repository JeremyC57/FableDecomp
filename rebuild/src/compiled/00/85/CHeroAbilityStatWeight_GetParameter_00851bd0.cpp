// CHeroAbilityStatWeight::GetParameter(const CTCHeroExperience&) const -- the weighted mean, over the stats the ability weights, of the hero's level in each stat as a fraction of that stat's maximum level (0 when the weights sum below 1e-4)
// Leaf: ?GetParameter@CHeroAbilityStatWeight@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00851BD0, 234 bytes, __fastcall, `ret 4`, float in st0.
// Identity: Ego_r.pdb / FableWin.pdb ?GetParameter@CHeroAbilityStatWeight@@QBEMABVCTCHeroExperience@@@Z, right after
//   CHeroAbilityStatPackage::IsValid (TLC 0x00851B50) in both PDBs and in retail; the landed GetRoughExperienceLevel
//   0x006D64B0 declares it at this address. The manifest label "CTCHeroExperience_GetAbilityMultiplier" is an autoname.
// Callees: CTCHeroExperience::GetTrainableStatLevel 0x006D6520, GetTrainableStatMaxLevel 0x006D6770 (both const, `ret 4`).
// Constants: the 1e-4f epsilon @ 0x0129BA3C is a named global (as in the landed 0x00830DD0 / 0x00ADEBC0): with a literal
//   VC7.1 threads the empty-map path straight to `return 0` (237 v 234); 0.0f @ 0x0122DEDC.
// Layout: StatWeights map<EHeroTrainableStatType, float> at +0 (STLport _Rb_tree; the landed 0x00756540 iterator model).
// Shape: the weight and the stat are locals read before the first call (the weight is homed in the dead argument slot);
//   the level is a float local spilled across the second call; two float sums zeroed at entry.
enum EHeroTrainableStatType { };

extern const float g_epsilon_0129ba3c;                               // 0x0129BA3C (1e-4f)

class CTCHeroExperience {
public:
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const;      // 0x006D6520
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const;   // 0x006D6770
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

class CHeroAbilityStatWeight {
public:
    float GetParameter(const CTCHeroExperience& experience) const;
    std::map<EHeroTrainableStatType, float> StatWeights;             // +0x00
};

float CHeroAbilityStatWeight::GetParameter(const CTCHeroExperience& experience) const
{
    float totalWeight = 0.0f;
    float total = 0.0f;
    for (std::map<EHeroTrainableStatType, float>::const_iterator it = StatWeights.begin(); it != StatWeights.end(); ++it) {
        EHeroTrainableStatType stat = it->first;
        float weight = it->second;
        float level = (float)experience.GetTrainableStatLevel(stat);
        total += level / experience.GetTrainableStatMaxLevel(stat) * weight;
        totalWeight += weight;
    }
    if (totalWeight < g_epsilon_0129ba3c)
        return 0.0f;
    return total / totalWeight;
}