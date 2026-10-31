// Grade: DIFFER(285v288) (15 difflines, was 30) -- the out-parameter iterator in its own block scope (`_Rb_tree_iterator found; { _Rb_tree_iterator it; ...; found = it; }`) frees its frame word, so the frame is retail's 0x18 and the 0x42 key/predicate slots line up; residue: this in ebp and the found node in ebx where retail has ebx/ebp, and retail re-reads the ability data (`mov ecx,[ebp+14h] ; fmul [ecx+8]`) for the duration where ours reuses the data register (re-reading it in source moves the allocation, 27/18) (retry wave 2 sec2b)
// Was: DIFFER(287v288) -- retail copies the found iterator's node into ebp and reuses the iterator's frame word for the 0x42 lookup key and the hit predicate (frame 0x18; this in ebx); ours keeps the address-taken iterator in its own word (frame 0x1c; this in ebp). Rest (seed, roll, GetTC, Bind3 filter, action checks) matches. Tried: ability reference (287v288), reads through it (291v288), pair reference (288v288), iterator copy (this file).
// CTCCombat::UpdateGetHitCounterAttack(const CThingCreatureBase& attacker) -- when the creature holds a counter-attack ability (type 9) that is active, or ready and wins its percentage roll, and the attacker's unblocked melee hit is in its event list while the current action can be interrupted: start the ability timer and finish the current action
// Leaf: ?UpdateGetHitCounterAttack@CTCCombat@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00733CF0, 288 bytes, __thiscall, `ret 4`.
// PDB: ?UpdateGetHitCounterAttack@CTCCombat@@QAEXABVCThingCreatureBase@@@Z (catalog "TCCombat_ApplyBlockResponse" was
//   descriptive).
// Callees: CTCCombat::GetCombatAbilityData 0x00733620, 0x00686A80 (the world accessor: called here without `this`, so a
//   this-less fold of CTCBase::GetWorld -- g_pMainGameComponent->+0x24), CWorld::GetPRandomSeed 0x0049D810, GFROR13
//   0x00497890, the const component-map LowerBound 0x004365B0, CTCEntityEvents::GetEvent<const CEventHitBy,
//   CFilter_Bind3<CIsHitEventHitByMelee, unary_negate<CIsHitEventBlocked>, CIsHitEventHitBy> > 0x00738A80,
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0 / FinishCurrentAction 0x00662650, the current action's slot 27
//   IsActionReadyForInterruption (+0x6C), g_FramesPerSecond 0x01375550, __ftol2.
// Layout: retail CTCCombat (STLport) with the combat-ability multimap at +0x70 as in the landed GetCombatAbilityData;
//   map node value: CCountedPointer<CCombatAbilityData> +0x14, CCombatAbility +0x1C (RepeatTimer) / +0x20 (ActiveTimer);
//   CCombatAbilityData PercentageChance +4, SecondsDuration +8; CThing present bits +0x20, interface map +0x44;
//   CThingCreatureBase PCurrentAction +0x100.
// Shape: the random seed is fetched before the ability test; the hit filter is CFilter_Bind3 holding its three
//   predicates by reference (the empty melee predicate homed in the attacker's argument slot).
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
    bool IsActive() const { return ActiveTimer >= 0; }
};
struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
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
struct _Rb_tree_iterator {                      // STLport 4.x tree iterator: the node pointer, null by default
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() : _M_node(0) {}
    SCombatAbilityValue& operator*() const { return ((_Rb_tree_node*)_M_node)->_M_value_field; }
    SCombatAbilityValue* operator->() const { return &(operator*()); }
};

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
inline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
class CWorld {
public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
CWorld* GetWorld_00686a80();                    // 0x00686A80 (this-less here)
extern long g_FramesPerSecond;                  // 0x01375550

class CTCBase_Retail;
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};

class CCreatureActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool IsActionReadyForInterruption() const;   // slot 27 (+0x6C)
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 UsingComponents
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                       // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
class CThingCreatureBase : public CThing {
public:
    unsigned char        _pad50[0x100 - 0x50];
    CCreatureActionBase* PCurrentAction;        // +0x100 (CCountedPointer<CCreatureActionBase>::Object)
    bool IsPerformingAnyAction() const;         // 0x006622A0
    void FinishCurrentAction();                 // 0x00662650
};

namespace std {
template <class _Predicate> class unary_negate {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) {}
};
template <class _Predicate> inline unary_negate<_Predicate> not1(const _Predicate& __pred) { return unary_negate<_Predicate>(__pred); }
}

template <class TFilter1, class TFilter2, class TFilter3> class CFilter_Bind3 {
public:
    const TFilter1& Filter1;
    const TFilter2& Filter2;
    const TFilter3& Filter3;
    CFilter_Bind3(const TFilter1& f1, const TFilter2& f2, const TFilter3& f3) : Filter1(f1), Filter2(f2), Filter3(f3) {}
};

namespace NEntityEvents {
class CEventHitBy;
class CIsHitEventHitByMelee { public: CIsHitEventHitByMelee() {} };
class CIsHitEventBlocked { public: CIsHitEventBlocked() {} };
class CIsHitEventHitBy {
public:
    const CThing* HitBy;
    CIsHitEventHitBy(const CThing& thing) : HitBy(&thing) {}
};
}

class CTCEntityEvents {
public:
    template <class TEvent, class TFilter> bool GetEvent(TEvent** event, const TFilter& filter);   // 0x00738A80
};

class CTCCombat {
public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x70 - 0x08];
    void*   CombatAbilities[3];                 // +0x70 STLport multimap
    bool GetCombatAbilityData(_Rb_tree_iterator& it, long interfaceType);   // 0x00733620
    void UpdateGetHitCounterAttack(const CThingCreatureBase& attacker);
};

void CTCCombat::UpdateGetHitCounterAttack(const CThingCreatureBase& attacker)
{
    _Rb_tree_iterator found;
    {
        _Rb_tree_iterator it;
        if (!GetCombatAbilityData(it, 9))
            return;
        found = it;
    }
    CCombatAbilityData* data = found->second.first.Object;
    unsigned long* seed = GetWorld_00686a80()->GetPRandomSeed();
    if (found->second.second.IsActive() || (found->second.second.RepeatTimer == -1 && GFRandom(seed) % 100 < (unsigned long)data->PercentageChance)) {
        if (ParentThing->HasTC(0x42)) {
            CTCEntityEvents* events = (CTCEntityEvents*)ParentThing->m_tcs.Find(0x42);
            const NEntityEvents::CEventHitBy* event;
            if (events->GetEvent(&event, CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee, std::unary_negate<NEntityEvents::CIsHitEventBlocked>,
                                                       NEntityEvents::CIsHitEventHitBy>(NEntityEvents::CIsHitEventHitByMelee(),
                                                                                        std::not1(NEntityEvents::CIsHitEventBlocked()),
                                                                                        NEntityEvents::CIsHitEventHitBy(attacker)))) {
                CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
                if (creature->IsPerformingAnyAction() && creature->PCurrentAction->IsActionReadyForInterruption()) {
                    found->second.second.ActiveTimer = (long)(g_FramesPerSecond * data->SecondsDuration);
                    creature->FinishCurrentAction();
                }
            }
        }
    }
}
