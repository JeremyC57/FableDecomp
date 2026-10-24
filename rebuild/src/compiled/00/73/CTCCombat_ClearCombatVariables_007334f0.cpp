// CTCCombat::ClearCombatVariables() -- reset the combat component's transient state: block/action flags off, combo chain position/id to their -777/-999 sentinels, grace and idle countdowns zeroed, every combat ability's repeat/active timers to -1, block timers to -1 and the component ready for melee
// Leaf: ?ClearCombatVariables@CTCCombat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007334F0, 140 bytes (144 with padding), `ret`.
// Label correction: catalog "CTCCombat_ResetState" -- Ego_r.pdb names ?ClearCombatVariables@CTCCombat@@AAEXXZ (private,
//   void); callers CTCCombat::RemoveFromCombat (landed 0x00733580, calls it "ResetCombatState") and 0x007355DE.
// Callees: none (STLport tree iterator ++ inlined).
// Layout: retail CTCCombat with 12-byte STLport vectors / multimap (engine/CTCCombat.h carries the PDB's 16-byte MSVC
//   containers, so every member from +0x2c on is shifted): PerformingAction +0x60 .. Blocking +0x63, BlockHoldTimer
//   +0x64, BlockingNumFrames +0x68, CombatAbilities +0x70 (node value +0x10: key, CCountedPointer (8), CCombatAbility
//   {RepeatTimer +0x1c, ActiveTimer +0x20}), ReadyForMelee +0x80; BlockCounterTimer +0x50.
// Shape: stores in source order (the tree header load is hoisted to the top); `_M_increment` inline with the right-
//   subtree descent and the parent climb; the end() header reloaded every iteration.  The per-ability reset is the
//   inline CCombatAbility::Reset() (Ego_r.pdb ?Reset@CCombatAbility@@QAEXXZ): through its `this` the two -1 stores stay
//   ahead of the `_M_right` load; written as direct member stores VC7.1 hoists the load above them (same length).
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
struct CCombatAbilityMap {                      // STLport multimap<ECombatAbilityDataType, SCombatAbilityPair>: 12 bytes
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned long       _M_node_count;          // +0x04
    unsigned long       _M_key_compare;         // +0x08 (empty less<>, padded)
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};

class CCombatManager;
class CThingCreatureBase;
struct CDefPointeeBase;

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
    void ClearCombatVariables();
};

void CTCCombat::ClearCombatVariables()
{
    StartingToBlock = false;
    PerformingAction = false;
    PerformingStrikeAction = false;
    ComboChainPos = -777;
    ComboChainID = -999;
    Blocking = false;
    ComboChainGracePeriodCountdown = 0;
    HandednessGracePeriodCountdown = 0;
    UnarmedCombatIdleCountdown = 0;
    for (CCombatAbilityMap::iterator it = CombatAbilities.begin(); it != CombatAbilities.end(); ++it) {
        it->second.second.Reset();
    }
    BlockHoldTimer = -1;
    BlockCounterTimer = -1;
    BlockingNumFrames = 0;
    ReadyForMelee = true;
}