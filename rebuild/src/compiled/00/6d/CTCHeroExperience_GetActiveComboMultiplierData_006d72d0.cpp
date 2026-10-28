// CTCHeroExperience::GetActiveComboMultiplierData() const -- the def's combo-multiplier entry for the hero's current experience multiplier: the first ComboMultiplierMap entry whose key bracket [previous key, key) holds the rounded multiplier, else the last entry
// Leaf: ?GetActiveComboMultiplierData@CTCHeroExperience@@ABEABVCComboMultiplierData@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D72D0, 243 bytes, thiscall `ret`, returns a const reference (eax = &entry.second).
// Identity (proven): Ego_r.pdb ?GetActiveComboMultiplierData@CTCHeroExperience@@ABEABVCComboMultiplierData@@XZ (donor 0x00588900,
//   0x100 bytes, between OnCreate and GetNextExperienceFallbackMultiplier); the body reads CTCHeroExperience members
//   (ExperienceMultiplierParam +0x30, PDef +0x54) and CHeroExperienceDef::ComboMultiplierMap (map<long, CComboMultiplierData>,
//   +0x58) / ExperienceMultiplierParamPerUnitComboMultiplier (+0x64); its seven callers are CTCHeroExperience members
//   (AddSuccessfulBlockParry 0x006D7490 first). The catalog label "BSTInterval_FindNode" is descriptive.
// Callees: __ftol2 @ 0x00BFEA70 (the double-to-long cast), the STLport const map iterator's operator++(int) @ 0x006DA200
//   (out of line, `ret 8`, hidden result).
// Layout: engine/CTCHeroExperience.h (retail STLport layout, PDef_Object +0x54). CHeroExperienceDef retail offsets: def base
//   0x28, ExperienceLevelThresholds / HeroLevelTitles / FallbackMultipliers +0x28/+0x34/+0x40, SoundIntensityThresholds +0x4c,
//   ComboMultiplierMap +0x58, ExperienceMultiplierParamPerUnitComboMultiplier +0x64 (Ego_r names, 12-byte STLport maps).
//   CComboMultiplierData (Ego_r): ComboMultiplier map, two floats (0x14 bytes retail).
// Shape: STLport 4.6 _Rb_tree_iterator with _Rb_global<bool>::_M_increment / _M_decrement expanded in place (the loop's ++it
//   and the final rbegin()), the first post-increment kept as the out-of-line call.
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience

extern "C" long __cdecl _ftol2(double);

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static __forceinline _Base_ptr __cdecl _M_increment(_Base_ptr _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
    static __forceinline _Base_ptr __cdecl _M_decrement(_Base_ptr _M_node)
    {
        if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
            _M_node = _M_node->_M_right;
        else if (_M_node->_M_left != 0) {
            _Base_ptr __y = _M_node->_M_left;
            while (__y->_M_right != 0)
                __y = __y->_M_right;
            _M_node = __y;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_left) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            _M_node = __y;
        }
        return _M_node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self operator++(int);                              // 0x006DA200 (out of line in retail)
    _Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
    _Node*       _M_header;                             // _Rb_tree_base::_M_header._M_data
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    const_iterator begin() const { return (_Node*)_M_header->_M_left; }
    const_iterator end() const { return _M_header; }
};
}   // namespace std

enum EDamageAttribute { EDamageAttribute_Force32 = 0x7fffffff };
class CComboMultiplierData {                            // Ego_r.pdb; 0x14 bytes retail
public:
    std::map<EDamageAttribute, float> ComboMultiplier;  // +0x00
    float ExperienceMultiplierNumSecondsForAttenuation; // +0x0c
    float ExperienceMultiplierParamDecayDeltaPerSecond; // +0x10
};
class CHeroExperienceDef {                              // retail offsets, Ego_r names
public:
    unsigned char _pad00[0x58];                         // def base, three vectors, SoundIntensityThresholds
    std::map<long, CComboMultiplierData> ComboMultiplierMap;    // +0x58
    float ExperienceMultiplierParamPerUnitComboMultiplier;      // +0x64
};

class CTCHeroExperience : public CTCHeroExperienceLayout {
    const CComboMultiplierData& GetActiveComboMultiplierData() const;
};

const CComboMultiplierData& CTCHeroExperience::GetActiveComboMultiplierData() const
{
    long multiplier = (long)(ExperienceMultiplierParam / ((const CHeroExperienceDef*)PDef_Object)->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
    if (multiplier < 1)
        multiplier = 1;
    const std::map<long, CComboMultiplierData>& combos = ((const CHeroExperienceDef*)PDef_Object)->ComboMultiplierMap;
    std::map<long, CComboMultiplierData>::const_iterator end = combos.end();
    std::map<long, CComboMultiplierData>::const_iterator first = combos.begin();
    long previous = (*first++).first;
    for (std::map<long, CComboMultiplierData>::const_iterator it = first; it != end; ++it) {
        long key = (*it).first;
        if (multiplier >= previous && multiplier < key)
            return (*it).second;
        previous = key;
    }
    std::map<long, CComboMultiplierData>::const_iterator last = end;
    --last;
    return (*last).second;
}