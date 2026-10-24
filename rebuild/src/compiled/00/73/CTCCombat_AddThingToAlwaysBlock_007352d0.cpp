// CTCCombat::AddThingToAlwaysBlock(const CThing* thing) -- remember a thing the creature should always block: wrap it in an intelligent pointer and append it to AlwaysBlockThings unless it is already there
// Leaf: ?AddThingToAlwaysBlock@CTCCombat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007352D0, 97 bytes (112 with padding), `ret 4`.
// Label correction: catalog "CombatBlockingData_AddIfNew" -- Ego_r.pdb ?AddThingToAlwaysBlock@CTCCombat@@QAEXPBVCThing@@@Z;
//   caller 0x0089E05E.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::SetPItem 0x00A01B90, ::~CBaseIntelligentPointer
//   0x00A01C10 (CIntelligentPointer<const CThing> vtable 0x01237F1C), STLport __find(first, last, val,
//   random_access_iterator_tag) 0x007379D0 (__fastcall, unrolled), vector<CIntelligentPointer<const CThing> >::push_back
//   0x007398B0 (out of line).
// Layout: retail CTCCombat::AlwaysBlockThings at +0x54 (STLport vector: start +0x54, finish +0x58).
// Shape: the intelligent pointer is a named local (base ctor, derived vtable, SetPItem); `end()` is read once into edi
//   before the find and compared after it; STLport `find` inlined down to the `__find` call with the iterator tag
//   temporary in the dead argument slot (the tag has a user constructor: a value-initialised empty POD tag adds a
//   `mov byte ptr [esp+1Ch],0`); the derived destructor is implicit (straight base destructor call).
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
    iterator lower_bound(const long& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const long& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const long& k) { return pair<iterator, iterator>(lower_bound(k), upper_bound(k)); }
};

struct CBaseObject;
class CThing;
class CBaseIntelligentPointer {                 // 8 bytes: vptr + handle
public:
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
    long m_handle;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C (const CThing)
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
typedef CIntelligentPointer<const CThing> TThingPtr;

struct random_access_iterator_tag { random_access_iterator_tag() {} };
TThingPtr* __fastcall __find(TThingPtr* first, TThingPtr* last, const TThingPtr& val, const random_access_iterator_tag&);   // 0x007379D0
inline TThingPtr* find(TThingPtr* first, TThingPtr* last, const TThingPtr& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}
struct CThingPtrVector {                        // STLport vector<CIntelligentPointer<const CThing> >: 12 bytes
    TThingPtr* _M_start;
    TThingPtr* _M_finish;
    TThingPtr* _M_end_of_storage;
    TThingPtr* begin() { return _M_start; }
    TThingPtr* end() { return _M_finish; }
    void push_back(const TThingPtr& x);         // 0x007398B0 (out of line)
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
    CThingPtrVector     AlwaysBlockThings;              // +0x54 STLport vector
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
    void AddThingToAlwaysBlock(const CThing* thing);
};

void CTCCombat::AddThingToAlwaysBlock(const CThing* thing)
{
    TThingPtr pointer(thing);
    TThingPtr* end = AlwaysBlockThings.end();
    if (find(AlwaysBlockThings.begin(), end, pointer) == end)
        AlwaysBlockThings.push_back(pointer);
}