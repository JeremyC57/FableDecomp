// CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility ability) const -- whether the hero may start an ability now
// Grade: DIFFER(584v584) -- same length, every instruction and the jump table agree except the 15 `!HasTC` case arms: ours loads ParentThing into a rotating scratch register (`mov ecx/edx/eax,[esi+4]; mov eax,[reg+30h]`), retail reuses esi (`mov esi,[esi+4]; mov eax,[esi+30h]`), 2 register bytes per arm
// Leaf: ?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072C430, 584 bytes (508 code + the 19-entry jump table at 0x0072C62C), `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb hold `?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z`; the FableWin donor
//   order puts it after CancelUsingAbility (0x0072ACF0) and IsAbleToUseAnyAbility (0x0072AF30, called first here, `QBE`);
//   UseSpecialAbility 0x0072CF40 calls it at 0x0072CF52 before the Setup* jump table. The catalog label
//   `GetAbilityAvailability` is wrong (no such PDB name).
// Callees: CTCSpecialAbilities::IsAbleToUseAnyAbility 0x0072AF30; the const TC-map CVectorMap::LowerBound 0x004365B0 (twice);
//   CTCInventoryAbilities::IsAbleToUseAbility 0x005EE49A; CTCTargetingBase slot 33 GetPTargetedThing (+0x84, result unused).
// Layout: no engine header models CThing's retail TC bitset (+0x20, std::bitset<274>) and TC map (+0x44); CTCSpecialAbilities
//   members are the Ego_r.pdb offsets; interface types come from each class's GetInterfaceType (slot 21).
// Shape: one `&&` guard wrapping the switch (separate early returns split the epilogue); PeekTC tests the bit through the
//   non-const bitset reference (retail `test ch,ch` / `test ch,1`) and stores through one ternary; the arms test it through
//   the const operator[] (retail `shr/not/and`). Case order follows the retail arm order. EHeroAbility values are from the
//   UseSpecialAbility jump table (each value -> its Setup* call); HEAL_LIFE = 14 is also in STS_BriarRose.lua.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public:   // retail STLport list (4 bytes)
    void* _M_node;
};
template <unsigned int _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static _WordT _S_maskbit(unsigned int __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <unsigned int _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    typedef _Base_bitset<(_Nb + 31) / 32> _Base;
    struct reference {
        typedef _Base_bitset<(_Nb + 31) / 32> _Bitset_base;
        unsigned long* _M_wp;
        unsigned int _M_bpos;
        reference(bitset<_Nb>& __b, unsigned int __pos)
        {
            _M_wp = &__b._M_w[_Bitset_base::_S_whichword(__pos)];
            _M_bpos = __pos % 32;
        }
        operator bool() const { return (*(_M_wp) & _Bitset_base::_S_maskbit(_M_bpos)) != 0; }
    };
    reference operator[](unsigned int __pos) { return reference(*this, __pos); }
    bool operator[](unsigned int __pos) const { return _Unchecked_test(__pos); }
    bool _Unchecked_test(unsigned int __pos) const
    {
        return (this->_M_getword(__pos) & _Base::_S_maskbit(__pos)) != (unsigned long)0;
    }
};
}   // namespace std

template <class T> class CArray : public std::vector<T> { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    value_type* LowerBound(const K& key);                     // 0x0040F020 for the TC map
    const value_type* LowerBound(const K& key) const;         // 0x004365B0 for the TC map
};

enum ETCInterfaceType { };                                  // values not needed here
enum EHeroAbility { };                                      // values (1..19) are listed in the case comments
class CTCBase;

class CThing { public:                                      // partial view, retail offsets
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];                        // CClass / CBaseObject / CBaseClass part
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    __forceinline bool HasTC(ETCInterfaceType type) const { return UsingComponents[type]; }   //HASTC
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::StatGetInterfaceType()]) {
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound(T::StatGetInterfaceType());
            if (it == map->Data.end() || T::StatGetInterfaceType() < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (const_cast<std::bitset<274>&>(UsingComponents)[T::StatGetInterfaceType()]) {
            const TCMap* map = &Components;
            const TCMap::value_type* it = map->LowerBound(T::StatGetInterfaceType());
            *out = (const T*)((it == map->Data.end() || T::StatGetInterfaceType() < it->first) ? map->Data.end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase;

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
class CCharString;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable { public:     // vtable 0x0125BE0C, 28 slots
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // slot 0; 0x00686830
    CThing& GetParentThing() { return *ParentThing; }
    const CThing& GetParentThing() const { return *ParentThing; }
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);                       // slot 1
public:
    virtual void OnRemove();                                                                // slot 2
    virtual void ConstructFromParams(const CTCParamsBase& params);                          // slot 3
    virtual void OnCreate();                                                                // slot 4
    virtual void InitialActivate();                                                         // slot 5
    virtual void OnInitialActivate();                                                       // slot 6
    virtual void RenderUpdate(float time);                                                  // slot 7
    virtual bool HasFrameUpdate() const;                                                    // slot 8
    virtual void FrameUpdate();                                                             // slot 9
    virtual void BulletTimeFrameUpdate();                                                   // slot 10
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // slot 11
    virtual void Copy(const CTCBase& other);                                                // slot 12
    virtual void OnDie();                                                                   // slot 13
    virtual void OnKill();                                                                  // slot 14
    virtual void OnLevelUnload();                                                           // slot 15
    virtual void OnSetInLimbo(bool inLimbo);                                                // slot 16
    virtual bool Validate(CCharString& error) const;                                        // slot 17
    virtual bool IsFreeForDeletion() const;                                                 // slot 18
    virtual bool IsSerialiseCreatable() const;                                              // slot 19
    virtual bool IsSerialisableForHeroState() const;                                        // slot 20
    virtual ETCInterfaceType GetInterfaceType() const = 0;                                  // slot 21
    virtual CCharString GetClassTypeName() const = 0;                                       // slot 22
    virtual bool IsComponentSerialisable() const;                                           // slot 23
    virtual bool IsDriver() const;                                                          // slot 24
    virtual bool IsCopyable() const;                                                        // slot 25
    virtual void GlobalUpdate(bool force);                                                  // slot 26
    virtual void HandleMessage(const NTCMessages::CMessageBase& message);                   // slot 27
};
class CTCForcePushPower : public CTCBase { public:                       // interface type 0x96
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x96; }
};
class CTCBulletTime : public CTCBase { public:                           // interface type 0x92
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x92; }
};
class CTCWillEnflame : public CTCBase { public:                          // interface type 0x95
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x95; }
};
class CTCTurncoatSpell : public CTCBase { public:                        // interface type 0x9E
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x9E; }
};
class CTCDrainLife : public CTCBase { public:                            // interface type 0x97
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x97; }
};
class CTCWillLightning : public CTCBase { public:                        // interface type 0x94
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x94; }
};
class CTCBerserk : public CTCBase { public:                              // interface type 0x84
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x84; }
};
class CTCBattleCharge : public CTCBase { public:                         // interface type 0xA8
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0xA8; }
};
class CTCMultiArrow : public CTCBase { public:                           // interface type 0x88
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x88; }
};
class CTCAssassinRush : public CTCBase { public:                         // interface type 0xA9
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0xA9; }
};
class CTCHealLife : public CTCBase { public:                             // interface type 0x99
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x99; }
};
class CTCGhostSword : public CTCBase { public:                           // interface type 0x9A
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x9A; }
};
class CTCFireballSpell : public CTCBase { public:                        // interface type 0x9D
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x9D; }
};
class CTCDivineWrath : public CTCBase { public:                          // interface type 0x85
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x85; }
};
class CTCUnholyPower : public CTCBase { public:                          // interface type 0x86
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x86; }
};
class CTCInventoryAbilities : public CTCBase { public:                   // interface type 0x6F
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x6F; }
    bool IsAbleToUseAbility(EHeroAbility ability, bool check_mutex) const;   // 0x005EE49A
};
class C3DVector;
enum ECreatureInteractionType { };                          // values not needed here
class CTCTargetingBase : public CTCBase { public:          // interface type 0x8; vtable 0x01271E74
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x8; }
    virtual CThing* GetPBestThingForMeleeAttack() const;                                    // slot 28
    virtual CThing* GetPBestThingForProjectileTargetingLock() const;                        // slot 29
    virtual CThingCreatureBase* GetPBestEnemyCreatureForAttack() const;                     // slot 30
    virtual CThingCreatureBase* GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType type, float minRange, float maxRange) const;   // slot 31
    virtual CThingCreatureBase* GetPBestCreatureForInteractionType(ECreatureInteractionType type) const;   // slot 32
    virtual CThing* GetPTargetedThing() const;                                              // slot 33 (+0x84)
    virtual void SetPTargetedThing(CThing* thing);                                          // slot 34
    virtual CThing* GetPProjectileTargetedThing() const;                                    // slot 35
    virtual CThing* GetPTargetLockedThing() const;                                          // slot 36
    virtual bool GetTargetedPos(C3DVector& pos, float distance) const;                      // slot 37
    virtual bool IsTargetedThingInRange(unsigned long range) const;                         // slot 38
protected:
    virtual void UpdateNearestInterestingThing();                                           // slot 39
};
class CTCSpecialAbilities : public CTCBase { public:      // interface type 0xA7
    class CUndeadFollower;
    bool                            AbilityMutexLock;       // +0x0C
    bool                            UsingBed;               // +0x0D
    EHeroAbility                    CurrentAbility;         // +0x10
    std::list<CUndeadFollower>      UndeadFollowers;        // +0x14
    CThingCreatureBase*             Creature;               // +0x18
    std::vector<bool>               AbilityAvailability;    // +0x1C (retail indexes it bytewise)
    bool IsAbilityAvailable(EHeroAbility ability) const;
    bool IsAbleToUseAnyAbility(EHeroAbility ability) const; // 0x0072AF30
};
class CThingCreatureBase : public CThing { };

bool CTCSpecialAbilities::IsAbilityAvailable(EHeroAbility ability) const
{
    const CTCInventoryAbilities* inventoryAbilities;
    if (IsAbleToUseAnyAbility(ability) && AbilityAvailability[ability] && ParentThing->PeekTC(&inventoryAbilities) && inventoryAbilities->IsAbleToUseAbility(ability, true)) {
    const CTCTargetingBase* targeting;
    Creature->PeekTC(&targeting);
    targeting->GetPTargetedThing();
    switch (ability) {
    case 2:  return !ParentThing->HasTC(CTCBulletTime::StatGetInterfaceType());
    case 1:  return !ParentThing->HasTC(CTCForcePushPower::StatGetInterfaceType());
    case 3:  return !ParentThing->HasTC(CTCWillEnflame::StatGetInterfaceType());
    case 5:  return !ParentThing->HasTC(CTCTurncoatSpell::StatGetInterfaceType());
    case 6:  return !ParentThing->HasTC(CTCDrainLife::StatGetInterfaceType());
    case 11: return !ParentThing->HasTC(CTCWillLightning::StatGetInterfaceType());
    case 8:  return !ParentThing->HasTC(CTCBerserk::StatGetInterfaceType());
    case 12: return !ParentThing->HasTC(CTCBattleCharge::StatGetInterfaceType());
    case 17: return !ParentThing->HasTC(CTCMultiArrow::StatGetInterfaceType());
    case 4:
    case 9:
    case 10: return true;
    case 13: return !ParentThing->HasTC(CTCAssassinRush::StatGetInterfaceType());
    case 14: return !ParentThing->HasTC(CTCHealLife::StatGetInterfaceType());
    case 15: return !ParentThing->HasTC(CTCGhostSword::StatGetInterfaceType());
    case 16: return !ParentThing->HasTC(CTCFireballSpell::StatGetInterfaceType());
    case 18: return !ParentThing->HasTC(CTCDivineWrath::StatGetInterfaceType());
    case 19: return !ParentThing->HasTC(CTCUnholyPower::StatGetInterfaceType());
    }
    }
    return false;
}
