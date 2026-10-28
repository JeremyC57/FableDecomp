// CCombatManager::UpdateAttackingCombatantsList(CCombatantInfo& info, bool force) -- per-frame pass over a defender's attackers: update its attack pattern and instruction, rebuild its placement wheel, then for each attacker on the wheel age its preferred dolly, run the sequence resets, and when its melee sequence's next action is a strike against an attackable defender, finish the strike it is still interruptibly playing
// Leaf: ?UpdateAttackingCombatantsList@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED030, 449 bytes, thiscall `ret 8`. Callers 0x006ED218, 0x006ED756, 0x006EDD78 (the
//   combat manager's register / update paths).
// Identity (proven): Ego_r.pdb ?UpdateAttackingCombatantsList@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z (donor 0x00716990) is,
//   with SetupDefenderPlacementWheel (donor 0x007167A0, the same (CCombatantInfo&, bool) signature), the only CCombatManager
//   member of this shape; the body calls retail 0x006ECEA0 with its own (info, bool) arguments, so 0x006ECEA0 is
//   SetupDefenderPlacementWheel and this is the list update that follows it in donor order. The manifest label
//   CCombatManager_UpdateDefenderState is descriptive and wrong.
// Callees: CCombatManager::UpdateDefenderAttackPattern(CCombatantInfo&) 0x006EA0C0, CCombatManager::UpdateDefenderAttackInstruction
//   (CCombatantInfo&) 0x006E86A0 (landed), NCreatureMode::IsAbleToBeAttacked(const CThing&) 0x0085FB30 (__fastcall),
//   CCombatManager::SetupDefenderPlacementWheel(CCombatantInfo&, bool) 0x006ECEA0, the STLport combatant tree's _M_find
//   0x006F0250 (inside the inlined private GetCombatantInfo(const CThingCreatureBase&)), CCombatManager::
//   UpdateAttackingCombatantSequenceResets(CCombatantInfo&) 0x006EABB0 (landed), CThingCreatureBase::IsInMeleeAsAttacker() const
//   0x00662410, CCombatSequenceBase::IsNextActionInSequenceStrikeAction() const 0x008B0750, CThingCreatureBase::
//   FinishCurrentAction() 0x00662650, std::vector<CThingCreatureBase*>::_M_insert_overflow 0x006F2680 (same-TU noinline STLport
//   body: malloc 0x00BFEA0E, memmove 0x00BFEAE6, free 0x00BFEA14).
// Virtuals: CCombatSequenceBase slots 7 IsMeleeSequence (+0x1C) and 8 IsBreakIntoMeleeSequence (+0x20) -- vtable 0x01278FEC slots
//   5..11 are the seven PDB bool defaults in declaration order (IsInterruptable .. IsAllowedOnOuterRing, each a test of slot 3
//   GetCombatSequenceType / 4 GetCombatSequenceInterruptionType / 15 GetZone; CCombatSequence_BanditBoast 0x012793AC slot 1
//   OnCreate news its actions, slot 2 GetName builds a CCharString, slot 3 returns 3 = boast); CCreatureActionBase slots 15 IsStrikeAction (+0x3C) and 27 IsActionReadyForInterruption (+0x6C), vtable 0x01259D24.
// Layout (Ego_r.pdb, retail STLport containers): CCombatManager Combatants +0x10; CCombatantInfo Parent +0x00, AttackerInfo +0x04
//   (PreferredAttackDollySet +0x10, PreferredAttackDollyTimer +0x14, UsingCombatWheel +0x20, CurrentSequence +0x24,
//   CombatSequences +0x28), DefenderInfo +0x34 (Attackers map: header +0x34, node count +0x38); tree node value +0x18; CThing
//   UniqueID +0x18; CThingCreatureBase PCurrentAction +0x100 (engine/CThingCreatureBase.h PCurrentAction_Data).
// Shape: the attackers are copied out of the map into a local STLport vector first (the per-attacker calls may change the map);
//   the defender's attackability is read once, before the wheel setup.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void* __cdecl malloc(unsigned int size);                                 // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef unsigned int size_type;
    typedef __true_type _IsPODType;
    vector() { }
    ~vector() { _M_clear(); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                                              // 0x006F2680
};
// The STLport 4.6 POD _M_insert_overflow body, same TU and not inlined (header code the retail TU saw too): with it visible VC7.1
// knows the local vector does not escape, so its finish stays in a register across the loop's calls, as retail does.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&,
                                                                  size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
#pragma auto_inline(on)

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        } else {
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
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, _Alloc> _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    iterator begin() { return iterator((_Link_type)_M_header._M_data->_M_left); }
    iterator end() { return iterator(_M_header._M_data); }
    bool empty() const { return _M_node_count == 0; }
    iterator find(const _Key& __x) { return iterator(_M_find(__x)); }
private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;                                      // 0x006F0250 (Combatants)
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    bool empty() const { return _M_t.empty(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
};
}   // namespace std

class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};

class CCreatureActionBase {                             // vtable 0x01259D24
public:
    virtual ~CCreatureActionBase();                     // slot 0
    virtual void Validate() const;                      // slot 1 (CBaseObject)
    virtual void EditUpdate(float);                     // slot 2
    virtual void PerformAction() = 0;                   // slot 3
    virtual void FinishAction();                        // slot 4
    virtual bool IsPlayingAnimation() const;            // slot 5
    virtual long GetAnimDelayTime() const;              // slot 6
    virtual long GetAnimationIndex() const;             // slot 7
    virtual long GetAnimationTransitionInTime() const;  // slot 8
    virtual long GetAnimationTransitionOutTime() const; // slot 9
    virtual unsigned long GetAnimExtraFlags() const;    // slot 10
    virtual CCreatureActionBase* Clone() const = 0;     // slot 11
    virtual void Initialise();                          // slot 12
    virtual bool IsStrikeEvent(long) const;             // slot 13
    virtual bool IsStrikeEventActive() const;           // slot 14
    virtual bool IsStrikeAction() const;                // slot 15 (+0x3C)
    virtual bool IsPartOfComboChain() const;            // slot 16
    virtual bool IsStrikeEventFinishedBeingActive() const;   // slot 17
    virtual bool IsVitalStrike() const;                 // slot 18
    virtual bool IsKnockdownStrike() const;             // slot 19
    virtual bool IsBlockCounter() const;                // slot 20
    virtual bool IsMeleeFlourish() const;               // slot 21
    virtual bool IsActionWithStrikeEventHelperName() const;  // slot 22
    virtual void GetStrikeEventHelperName() const;      // slot 23 (returns CCharString; not called here)
    virtual bool IsOverrideableByActionOfSamePriority() const;   // slot 24
    virtual void OnFrameUpdate(float);                  // slot 25
    virtual bool IsQueuable() const;                    // slot 26
    virtual bool IsActionReadyForInterruption() const;  // slot 27 (+0x6C)
};

class CThing {
public:
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UniqueID;                          // +0x18
    unsigned __int64 GetUniqueID() const { return UniqueID; }
};
class CThingCreatureBase : public CThing {
public:
    unsigned char                         _pad_0x20[0xe0];
    CCountedPointer<CCreatureActionBase>  PCurrentAction;   // +0x100
    const CCreatureActionBase* PeekPCurrentAction() const { return PCurrentAction.Get(); }
    bool IsInMeleeAsAttacker() const;                   // 0x00662410
    void FinishCurrentAction();                         // 0x00662650
};

namespace NCreatureMode {
bool __fastcall IsAbleToBeAttacked(const CThing& thing);   // 0x0085FB30
}

enum ECombatSequenceType { ECombatSequenceType_Force32 = 0x7fffffff };
enum ECombatSequenceInterruptionType { ECombatSequenceInterruptionType_Force32 = 0x7fffffff };
class CCombatSequenceBase {                             // vtable 0x01278FEC
public:
    virtual ~CCombatSequenceBase();                     // slot 0
    virtual void OnCreate() = 0;                        // slot 1 (pure in retail; the derived bodies new their actions)
    virtual void GetName() const = 0;                   // slot 2 (returns CCharString by value; not called here)
    virtual ECombatSequenceType GetCombatSequenceType() const;                         // slot 3 (+0x0C)
    virtual ECombatSequenceInterruptionType GetCombatSequenceInterruptionType() const; // slot 4 (+0x10)
    virtual bool IsInterruptable() const;               // slot 5
    virtual bool IsInterruptableDueToZoneChange() const;     // slot 6
    virtual bool IsMeleeSequence() const;               // slot 7 (+0x1C)
    virtual bool IsBreakIntoMeleeSequence() const;      // slot 8 (+0x20)
    bool IsNextActionInSequenceStrikeAction() const;    // 0x008B0750
};

class C2DVector { public: float X, Y; };
class CAttackingCombatantInfo {
public:
    CThingCreatureBase* Attacked;                       // +0x00
    long                AttackerNumber;                 // +0x04
    long                CombatWheelRingIndex;           // +0x08
    bool                PreferredAttackDollySet;        // +0x0C
    long                PreferredAttackDollyTimer;      // +0x10
    C2DVector           PreferredAttackDollyVector;     // +0x14
    bool                UsingCombatWheel;               // +0x1C
    long                CurrentSequence;                // +0x20
    std::vector<CCountedPointer<CCombatSequenceBase> > CombatSequences;   // +0x24
};
class CDefendingCombatantInfo {
public:
    std::map<unsigned __int64, CThingCreatureBase*> Attackers;   // +0x00 (CombatWheel and the rest follow; not used here)
};
struct CCombatantInfo {
    CThingCreatureBase*     Parent;                     // +0x00
    CAttackingCombatantInfo AttackerInfo;               // +0x04
    CDefendingCombatantInfo DefenderInfo;               // +0x34
};

class CCombatManager {
public:
    virtual ~CCombatManager();
    unsigned char                                   RegisteredCombatSequences[0xc];   // +0x04 (map<CCharString, creator>)
    std::map<unsigned __int64, CCombatantInfo>      Combatants;                       // +0x10
private:
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature)
    {
        return (*Combatants.find(creature.GetUniqueID())).second;
    }
    void UpdateDefenderAttackInstruction(CCombatantInfo& info);              // 0x006E86A0
    void UpdateDefenderAttackPattern(CCombatantInfo& info);                  // 0x006EA0C0
    void UpdateAttackingCombatantSequenceResets(CCombatantInfo& info);       // 0x006EABB0
    void SetupDefenderPlacementWheel(CCombatantInfo& info, bool force);      // 0x006ECEA0
    void UpdateAttackingCombatantsList(CCombatantInfo& info, bool force);
};

void CCombatManager::UpdateAttackingCombatantsList(CCombatantInfo& info, bool force)
{
    if (info.DefenderInfo.Attackers.empty())
        return;
    UpdateDefenderAttackPattern(info);
    UpdateDefenderAttackInstruction(info);
    bool defender_attackable = NCreatureMode::IsAbleToBeAttacked(*info.Parent);
    SetupDefenderPlacementWheel(info, force);

    std::vector<CThingCreatureBase*> attackers;
    for (std::map<unsigned __int64, CThingCreatureBase*>::iterator it = info.DefenderInfo.Attackers.begin();
         it != info.DefenderInfo.Attackers.end(); ++it)
        attackers.push_back(it->second);

    for (std::vector<CThingCreatureBase*>::iterator it = attackers.begin(); it != attackers.end(); ++it) {
        CThingCreatureBase* attacker = *it;
        CCombatantInfo& attacker_info = GetCombatantInfo(*attacker);
        if (!attacker_info.AttackerInfo.UsingCombatWheel)
            continue;
        if (attacker_info.AttackerInfo.PreferredAttackDollySet && --attacker_info.AttackerInfo.PreferredAttackDollyTimer < 0)
            attacker_info.AttackerInfo.PreferredAttackDollySet = false;
        if (attacker_info.AttackerInfo.CurrentSequence < 0)
            continue;
        UpdateAttackingCombatantSequenceResets(attacker_info);
        if (attacker->IsInMeleeAsAttacker() && attacker_info.AttackerInfo.CurrentSequence >= 0) {
            CCombatSequenceBase* sequence = attacker_info.AttackerInfo.CombatSequences[attacker_info.AttackerInfo.CurrentSequence].Get();
            if ((sequence->IsMeleeSequence() || sequence->IsBreakIntoMeleeSequence()) && defender_attackable &&
                sequence->IsNextActionInSequenceStrikeAction()) {
                const CCreatureActionBase* action = attacker->PeekPCurrentAction();
                if (action && action->IsStrikeAction() && action->IsActionReadyForInterruption())
                    attacker->FinishCurrentAction();
            }
        }
    }
}