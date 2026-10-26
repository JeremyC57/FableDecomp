// CTCCombat::GetBlockAbilityIterator(iterator& it, EWeaponClass weaponClass) -- walk the block-type combat abilities (key 1) with the caller's iterator and stop at the first whose blocking data is for the given weapon class: true with `it` on it, else false
// Leaf: ?GetBlockAbilityIterator@CTCCombat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00734340, 201 bytes (208 with padding), `ret 8`.
// Label correction: catalog "CTCCombat_FindBlockAbilityPrioritized" -- Ego_r.pdb has the non-const
//   ?GetBlockAbilityIterator@CTCCombat@@AAE_NAAV?$_Tree_iterator...@W4EWeaponClass@@@Z (bool, iterator&, enum);
//   callers 0x00734893 and 0x007349D0.  (The landed 0x00733750 is a different body that calls 0x00733620.)
// Callees: the ability data's virtual slot 3 (+0x0c, its data type; 1 = blocking).
// Layout: retail CTCCombat::CombatAbilities multimap +0x70 (node key +0x10, CCountedPointer<CCombatAbilityData> +0x14);
//   CCombatAbilityBlockingData::WeaponClass +0x2c (Ego_r.pdb +0x30 less the 4 bytes of the STLport set).
// Shape: `last = upper_bound(1)` walked first into ebp, then `for (it = lower_bound(1); it != last; ++it)` through the
//   caller's reference (stored on entry and after every `_Rb_global::_M_increment`); the data pointer is a named
//   local (esi) shared by the virtual call and the WeaponClass test.
class CCombatAbilityData {                      // Ego_r.pdb, 0x18 bytes
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual long GetDataType() const;           // slot 3 (+0x0c): 1 for the blocking data (descriptive name)
    long   PercentageChance;                    // +0x04
    float  SecondsDuration;                     // +0x08
    float  SecondsToWaitBeforeRepeat;           // +0x0c
    void*  Anim;                                // +0x10 CDefString
    long   InterfaceType;                       // +0x14 ECombatAbilityInterfaceType
};
class CCombatAbilityBlockingData : public CCombatAbilityData {   // Ego_r.pdb 0x34; retail 0x30 (STLport set)
public:
    float         BlockAngle;                   // +0x18
    bool          BlockCanBeBroken;             // +0x1c
    bool          ForceRecoilWhenBlocking;      // +0x1d
    void*         ValidBlockWeaponTypes[3];     // +0x20 STLport set<EWeaponClass>
    long          WeaponClass;                  // +0x2c EWeaponClass
};
template <class T> class CCountedPointer { public:     // 8 bytes: object + count block
    T*    Object;
    long* Count;
    T* operator->() const { return Object; }
};
class CCombatAbility { public:                         // Ego_r.pdb, 8 bytes
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
    iterator lower_bound(const long& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const long& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const long& k) { return pair<iterator, iterator>(lower_bound(k), upper_bound(k)); }
};

class CCombatManager;
class CThingCreatureBase;
class CDefPointeeBase;

class CTCCombat {                               // retail layout (STLport containers), 0x98 bytes
public:
    virtual void v00();
    void*               ParentThing;                    // +0x04 (CTCBase)
    bool                ToBeRemoved;                    // +0x08
    bool                IsInGlobalUpdateTCs;            // +0x09
    long                ComboChainPos;                  // +0x0c
    long                ComboChainID;                   // +0x10
    long                ComboChainGracePeriodCountdown; // +0x14
    long                HandednessGracePeriodCountdown; // +0x18
    long                UnarmedCombatIdleCountdown;     // +0x1c
    CCombatManager*     CombatManager;                  // +0x20
    CThingCreatureBase* ParentCreature;                 // +0x24
    CDefPointeeBase*    PCombatTypeDef;                 // +0x28
    void*               AbilityList[3];                 // +0x2c STLport vector
    void*               UsableAbilityList[3];           // +0x38 STLport vector
    long                CurrentAbilityIndex;            // +0x44
    CDefPointeeBase*    PBlockCounterAbility;           // +0x48
    CDefPointeeBase*    PFlourish360Ability;            // +0x4c
    long                BlockCounterTimer;              // +0x50
    void*               AlwaysBlockThings[3];           // +0x54 STLport vector
    bool                PerformingAction;               // +0x60
    bool                PerformingStrikeAction;         // +0x61
    bool                StartingToBlock;                // +0x62
    bool                Blocking;                       // +0x63
    long                BlockHoldTimer;                 // +0x64
    long                BlockingNumFrames;              // +0x68
    long                UnsheathingWeaponType;          // +0x6c
    CCombatAbilityMap   CombatAbilities;                // +0x70
    long                ShotOverrideType;               // +0x7c
    bool                ReadyForMelee;                  // +0x80
    long                CurFlourishTimer;               // +0x84
    void*               FlourishParticles[3];           // +0x88 STLport vector
    bool                IsInMiddleOfFlourishAction;     // +0x94
    bool GetBlockAbilityIterator(CCombatAbilityMap::iterator& it, long weaponClass);
};

bool CTCCombat::GetBlockAbilityIterator(CCombatAbilityMap::iterator& it, long weaponClass)
{
    CCombatAbilityMap::iterator last = CombatAbilities.upper_bound(1);
    for (it = CombatAbilities.lower_bound(1); it != last; ++it) {
        CCombatAbilityData* data = it->second.first.Object;
        if (data->GetDataType() == 1 && ((CCombatAbilityBlockingData*)data)->WeaponClass == weaponClass)
            return true;
    }
    return false;
}