// CTCHeroExperience::GetOnHitSoundIntensity() const -- scale the highest on-hit sound-intensity threshold by how far the combo multiplier has climbed from the current fallback towards the next one
// Leaf: ?GetOnHitSoundIntensity@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D86D0, length 153 bytes (+ int3 padding), thiscall plain `ret`.
// Ego_r.pdb `?GetOnHitSoundIntensity@CTCHeroExperience@@ABEJXZ`; single caller 0x006D8BF5, GetOnHitSound (0x006D8BF0, this
// chunk). The manifest label "Flourish_UpdateMultiplier" is descriptive and wrong.
// Callees: GetNextExperienceFallbackMultiplier() const @ 0x006D7430 and GetExperienceFallbackMultiplierForMultiplier(long)
// const @ 0x006D73D0 (both this chunk), __ftol2 @ 0x00BFEA70 (x3), the map const_iterator operator--() @ 0x006DA140
// (landed; STLport `_M_decrement` body, `return *this`). Constant: 0.5 (double) @ 0x01237F00.
// Layout: CTCHeroExperience retail (ExperienceMultiplierParam +0x30, PDef +0x54, as in the landed 0x006D7F20);
// CHeroExperienceDef retail (CDefClassBase 0x28 bytes, 12-byte STLport vectors / maps): FallbackMultipliers +0x40,
// SoundIntensityThresholds map<long, CDefString> +0x4C, ExperienceMultiplierParamPerUnitComboMultiplier +0x64.
// Shape: GetExperiencePickUpMultiplier / GetExperienceFallbackMultiplier are the inline accessors of the landed
// 0x006D7F20 (`(long)(param / perUnit + 0.5)` clamped to 1); the top threshold is `rbegin()->first` (STLport
// reverse_iterator: copy end() into a frame word, `--tmp`, dereference); the result is
// `(long)(top - top * (float)(next - 1 - multiplier) / (next - 1 - fallback))`.
// Pragma: the iterator's operator-- body (the landed 0x006DA140) sits above under `#pragma auto_inline(off)`, so it stays
// a call but VC7.1 knows it only writes the temporary: PDef stays in edi across it (with a bare declaration VC reloads
// [esi+54h] after the call, 154 bytes).
class CDefString { public: long Index; };

namespace std {

struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};

template <class K, class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    K first;                                                // +0x10
    V second;                                               // +0x14
};

template <class K, class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator& operator--();                        // 0x006DA140
    _Rb_tree_node<K, V>& operator*() const { return *(_Rb_tree_node<K, V>*)_M_node; }
};

template <class It, class Value> struct reverse_iterator {
    It current;
    reverse_iterator(It x) : current(x) { }
    Value& operator*() const { It tmp = current; return *--tmp; }
    Value* operator->() const { return &(operator*()); }
};

#pragma auto_inline(off)
template <class K, class V> _Rb_tree_iterator<K, V>& _Rb_tree_iterator<K, V>::operator--()
{
    _Rb_tree_node_base* x = _M_node;
    if (x->_M_color == 0 && x->_M_parent->_M_parent == x)
        x = x->_M_right;
    else if (x->_M_left != 0) {
        _Rb_tree_node_base* y = x->_M_left;
        while (y->_M_right != 0)
            y = y->_M_right;
        x = y;
    }
    else {
        _Rb_tree_node_base* y = x->_M_parent;
        while (x == y->_M_left) {
            x = y;
            y = y->_M_parent;
        }
        x = y;
    }
    _M_node = x;
    return *this;
}
#pragma auto_inline(on)

template <class K, class V> class map {
public:
    typedef _Rb_tree_iterator<K, V> const_iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    unsigned char       _M_key_compare[4];                  // +0x08
    const_iterator end() const { return const_iterator(_M_header); }
    typedef reverse_iterator<const_iterator, _Rb_tree_node<K, V> > const_reverse_iterator;
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
};

}

class CHeroExperienceDef {
public:
    unsigned char _base[0x28];                              // CDefClassBase
    long* ExperienceLevelThresholds[3];                     // +0x28
    long* HeroLevelTitles[3];                               // +0x34
    long* FallbackMultipliers[3];                           // +0x40
    std::map<long, CDefString> SoundIntensityThresholds;    // +0x4C
    unsigned char ComboMultiplierMap[0xC];                  // +0x58
    float ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
};

class CTCHeroExperience {
public:
    virtual void v00();
    void* ParentThing;                                      // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long FishingLevel;                                      // +0x0C
    long TotalExperiencePoints;                             // +0x10
    long ExperiencePointsAvailableToSpend;                  // +0x14
    long* StatExperiencePoints[3];                          // +0x18
    long* TrainableStatLevels[3];                           // +0x24
    float ExperienceMultiplierParam;                        // +0x30
    float LastExperienceMultiplierParam;                    // +0x34
    long ExperienceAttenuationTimer;                        // +0x38
    bool AbleToGainExperience;                              // +0x3C
    void* Pickups;                                          // +0x40
    long* ExperienceSpentOnStats[3];                        // +0x44
    void* AttackHistory;                                    // +0x50
    CHeroExperienceDef* PDef;                               // +0x54

    long GetExperiencePickupMultiplierForParam(float param) const
    {
        long multiplier = (long)(param / PDef->ExperienceMultiplierParamPerUnitComboMultiplier + 0.5);
        if (multiplier < 1)
            multiplier = 1;
        return multiplier;
    }
    long GetExperiencePickUpMultiplier() const { return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam); }
    long GetExperienceFallbackMultiplierForMultiplier(long multiplier) const;   // 0x006D73D0
    long GetExperienceFallbackMultiplier() const { return GetExperienceFallbackMultiplierForMultiplier(GetExperiencePickUpMultiplier()); }
    long GetNextExperienceFallbackMultiplier() const;       // 0x006D7430
    long GetOnHitSoundIntensity() const;
};

long CTCHeroExperience::GetOnHitSoundIntensity() const
{
    long top = GetNextExperienceFallbackMultiplier() - 1;
    long fallback = GetExperienceFallbackMultiplier();
    long maxIntensity = PDef->SoundIntensityThresholds.rbegin()->first;
    long multiplier = GetExperiencePickUpMultiplier();
    return (long)(maxIntensity - maxIntensity * (float)(top - multiplier) / (top - fallback));
}