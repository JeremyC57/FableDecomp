// GetArmourThresholdValueFromArmourWeights(EDamageAttribute, const multimap<CDefPointer<const CArmourDef>, float>&, float) -- the weighted armour threshold for a damage attribute: sum over the worn armour of threshold(attribute) * weight / total weight (0 when an armour has no threshold for it)
// Leaf: ?GetArmourThresholdValueFromArmourWeights@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
// Retail 0x008686E0, 0xB9 bytes (+ int3 padding to 0xC0), __fastcall (attribute in ecx, the weights in edx, the total
// weight on the stack, `ret 4`), float in st0. FableWin/Ego_r name (a file-static of the combat-damage TU):
// ?GetArmourThresholdValueFromArmourWeights@?A0x23f4dc30@@YIMW4EDamageAttribute@@ABV?$multimap@V?$CDefPointer@$$CBVCArmourDef@@@@MV?$CCompareDefPointer@$$CBVCArmourDef@@M@@...@std@@M@Z.
// Caller: 0x00869207 (the hit-damage evaluation that compares the result with a threshold field and sets a flag).
// Layout (retail): CArmourDef AugmentationResponse +0x3C, DamageTypeResponse +0x48, ArmourThresholds +0x54 (Ego_r
// +0x40/+0x50/+0x60: retail STLport maps are 12 bytes {header node pointer, node count, empty compare}); the weights
// multimap node holds the CDefPointer (the def pointer itself, +0x10) and the weight float (+0x14). Constants 0.0f @
// 0x0122DEDC, 1.0f @ 0x0122DED8.
// Shape: STLport iteration with _Rb_global<bool>::_M_increment inlined; per armour `float weight = it->second;` (loaded
// before the lookup), the threshold defaults to 0.0f and `it->first->ArmourThresholds.find(attribute)` is compared with
// `it->first->ArmourThresholds.end()` written out again (the def pointer stays in ebx, the map header is re-read; a
// reference local to the map keeps its address in edx instead); `/ totalWeight` becomes the loop-invariant reciprocal
// (`fld 1.0 ; fdiv [esp+0Ch]` after the empty test) times value times weight.
// Manifest label "CTCBase_CalculateDamageFromArmourTree" is a descriptive auto label.
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0 };

namespace std {
template <class _T1, class _T2> struct pair
{
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };

struct _Rb_tree_node_base
{
    bool                _M_color;                    // +0x00
    _Rb_tree_node_base* _M_parent;                   // +0x04
    _Rb_tree_node_base* _M_left;                     // +0x08
    _Rb_tree_node_base* _M_right;                    // +0x0C
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base
{
    _Value _M_value_field;                           // +0x10
};
template <class _Dummy> struct _Rb_global
{
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x)
    {
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        return __x;
    }
};
template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& __x) const { return _M_node == __x._M_node; }
    bool operator!=(const _Rb_tree_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _Key, class _Tp, class _Compare> class map      // also the layout of multimap
{
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    typedef _Rb_tree_node<value_type> _Node;
    _Rb_tree_node_base* _M_header;                   // +0x00
    unsigned long       _M_node_count;               // +0x04
    _Compare            _M_key_compare;              // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
    const_iterator find(const _Key& __k) const
    {
        _Rb_tree_node_base* __y = _M_header;            // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_header->_M_parent; // root
        while (__x != 0)
            if (!(((_Node*)__x)->_M_value_field.first < __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        if (__y == _M_header || __k < ((_Node*)__y)->_M_value_field.first)
            __y = _M_header;
        return const_iterator(__y);
    }
};
template <class _Key, class _Tp, class _Compare> class multimap : public map<_Key, _Tp, _Compare> { };
}

template <class T> class CDefPointer
{
public:
    T* Object;                                       // +0x00 (CDefPointeeBase*, the def at offset 0)
    T* operator->() const { return Object; }
};

template <class T, class V> class CCompareDefPointer { };

class CArmourDef
{
public:
    unsigned char _pad0[0x3c];
    std::map<EObjectAugmentationType, float, std::less<EObjectAugmentationType> > AugmentationResponse;   // +0x3C
    std::map<EDamageAttribute, float, std::less<EDamageAttribute> >               DamageTypeResponse;     // +0x48
    std::map<EDamageAttribute, float, std::less<EDamageAttribute> >               ArmourThresholds;       // +0x54
    bool AllHitsKnockdown;                           // +0x60
    bool AllHitsNegated;                             // +0x61
    bool AllHitsCauseRecoil;                         // +0x62
    long DamageMaterial;                             // +0x64
    long BlockMaterial;                              // +0x68
};

typedef std::multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<const CArmourDef, float> > TArmourWeights;
typedef std::map<EDamageAttribute, float, std::less<EDamageAttribute> > TDamageAttributeMap;
#pragma auto_inline(off)
namespace {
float __fastcall GetArmourThresholdValueFromArmourWeights(EDamageAttribute attribute, const TArmourWeights& weights, float totalWeight)
{
    float threshold = 0.0f;
    for (TArmourWeights::const_iterator it = weights.begin(); it != weights.end(); ++it)
    {
        float weight = it->second;
        float value = 0.0f;
        TDamageAttributeMap::const_iterator found = it->first->ArmourThresholds.find(attribute);
        if (found != it->first->ArmourThresholds.end())
            value = found->second;
        threshold += value * weight / totalWeight;
    }
    return threshold;
}
}
#pragma auto_inline(on)

float __fastcall CallGetArmourThresholdValueFromArmourWeights(EDamageAttribute attribute, const TArmourWeights& weights, float totalWeight)
{
    return GetArmourThresholdValueFromArmourWeights(attribute, weights, totalWeight);
}