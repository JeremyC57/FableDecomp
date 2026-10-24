// GetArmourResponseAugmentationDamageMultiplier(EObjectAugmentationType, const multimap<CDefPointer<const CArmourDef>, float>&, float, float&) -- scale the damage multiplier by the worn armour's object augmentation response: multiplier = sum over the armour of response * weight / total weight * multiplier (response 1 when an armour has none); always true
// Leaf: ?GetArmourResponseAugmentationDamageMultiplier@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
// Retail 0x00868A80, 0xBD bytes (+ int3 padding to 0xC0), __fastcall (object augmentation in ecx, the weights in edx; the total weight and
// the in/out multiplier reference on the stack, `ret 8`), returns `mov al,1`. FableWin/Ego_r name (a file-static of the
// combat-damage TU): ?GetArmourResponseAugmentationDamageMultiplier@?A0x23f4dc30@@YI_NW4EObjectAugmentationType@@ABV?$multimap@V?$CDefPointer@$$CBVCArmourDef@@@@MV?$CCompareDefPointer@$$CBVCArmourDef@@M@@...@std@@MAAM@Z.
// Caller: 0x00869274 (once per weapon augmentation), in the hit-damage evaluation next to GetArmourThresholdValueFromArmourWeights (0x008686E0).
// Layout (retail): CArmourDef AugmentationResponse +0x3C (12-byte STLport maps: AugmentationResponse +0x3C, DamageTypeResponse +0x48,
// ArmourThresholds +0x54); the weights multimap node holds the def pointer (+0x10) and the weight float (+0x14).
// Constants 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Shape: `float scale = it->second / totalWeight * multiplier;` per armour (the reciprocal hoisted after the empty test,
// the multiplier re-read through its reference every pass), the response defaults to 1.0f and
// `it->first->AugmentationResponse.find(...)` is compared with `it->first->AugmentationResponse.end()` written out again; `result += response *
// scale`, then `multiplier = result; return true;` (the empty map stores 0.0f).
// Manifest label "CTCBase_AccumulateMeleeBuffFromTree" is a descriptive auto label.
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };
enum EObjectAugmentationType { OBJECT_AUGMENTATION_NONE = 0 };

namespace _STL {
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
    _STL::map<EObjectAugmentationType, float, _STL::less<EObjectAugmentationType> > AugmentationResponse;   // +0x3C
    _STL::map<EDamageAttribute, float, _STL::less<EDamageAttribute> >               DamageTypeResponse;     // +0x48
    _STL::map<EDamageAttribute, float, _STL::less<EDamageAttribute> >               ArmourThresholds;       // +0x54
    bool AllHitsKnockdown;                           // +0x60
    bool AllHitsNegated;                             // +0x61
    bool AllHitsCauseRecoil;                         // +0x62
    long DamageMaterial;                             // +0x64
    long BlockMaterial;                              // +0x68
};

typedef _STL::multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<const CArmourDef, float> > TArmourWeights;
typedef _STL::map<EDamageAttribute, float, _STL::less<EDamageAttribute> > TDamageAttributeMap;
#pragma auto_inline(off)
namespace {
bool __fastcall GetArmourResponseAugmentationDamageMultiplier(EObjectAugmentationType attribute, const TArmourWeights& weights, float totalWeight, float& multiplier)
{
    float result = 0.0f;
    for (TArmourWeights::const_iterator it = weights.begin(); it != weights.end(); ++it)
    {
        float scale = it->second / totalWeight * multiplier;
        float response = 1.0f;
        _STL::map<EObjectAugmentationType, float, _STL::less<EObjectAugmentationType> >::const_iterator found = it->first->AugmentationResponse.find(attribute);
        if (found != it->first->AugmentationResponse.end())
            response = found->second;
        result += response * scale;
    }
    multiplier = result;
    return true;
}
}
#pragma auto_inline(on)

bool __fastcall CallGetArmourResponseAugmentationDamageMultiplier(EObjectAugmentationType attribute, const TArmourWeights& weights, float totalWeight, float& multiplier)
{
    return GetArmourResponseAugmentationDamageMultiplier(attribute, weights, totalWeight, multiplier);
}