// CTCCombat::OnKnockDown() -- the creature was knocked down: reset the repeat/active timers of every block-type combat ability, switch the creature's body reorienter on, and cancel any mutex special ability it is using
// Leaf: ?OnKnockDown@CTCCombat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007340B0, 221 bytes (224 with padding), `ret`.
// Label correction: catalog "TCBase_InvalidateAllAbilityTimers" -- the only caller is NCreatureMode::CModeKnockedDown::
//   OnActivate (0x008D9504, landed 0x008D9480) and Ego_r.pdb has ?OnKnockDown@CTCCombat@@QAEXXZ.
// Callees: CThingCreatureBase::SetBodyReorienterActive(bool) 0x00663490, CVectorMap<ETCInterfaceType,CTCBase*>::
//   LowerBound 0x0040F020 (interface 0xA7 = CTCSpecialAbilities, bit 7 of the presence byte +0x34: `test cl,cl ; jns`),
//   CTCSpecialAbilities::CancelCurrentMutexAbility 0x0072B420 (landed).
// Layout: retail CTCCombat (STLport containers): ParentCreature +0x24, CombatAbilities multimap +0x70 (node key +0x10,
//   CCombatAbility {RepeatTimer +0x1c, ActiveTimer +0x20}).
// Shape: the same block-ability walk as SetBlockBroken 0x00733F70 (`last = upper_bound(1)`, then from lower_bound(1),
//   CCombatAbility::Reset() inline, `_Rb_global::_M_increment` inline); the key 1 stays in ebx and is pushed again
//   as the `true`; the special-abilities lookup is the out-pointer GetInterface in the `push ecx` frame word.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CCombatAbilityData { public:                     // Ego_r.pdb, 0x18 bytes
    void*  __vftable;                           // +0x00
    long   PercentageChance;                    // +0x04
    float  SecondsDuration;                     // +0x08
    float  SecondsToWaitBeforeRepeat;           // +0x0c
    void*  Anim;                                // +0x10 CDefString
    long   InterfaceType;                       // +0x14 ECombatAbilityInterfaceType
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
class CDefPointeeBase;

class CTCSpecialAbilities {
public:
    void CancelCurrentMutexAbility();           // 0x0072B420
};
class CThingCreatureBase : public CThing {
public:
    void SetBodyReorienterActive(bool active);  // 0x00663490
};

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
    void OnKnockDown();
};

void CTCCombat::OnKnockDown()
{
    CCombatAbilityMap::iterator last = CombatAbilities.upper_bound(1);
    for (CCombatAbilityMap::iterator it = CombatAbilities.lower_bound(1); it != last; ++it)
        it->second.second.Reset();
    ParentCreature->SetBodyReorienterActive(true);
    CTCSpecialAbilities* abilities;
    if (ParentCreature->GetInterface(&abilities, 0xA7))
        abilities->CancelCurrentMutexAbility();
}