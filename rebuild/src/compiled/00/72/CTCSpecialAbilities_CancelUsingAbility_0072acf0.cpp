// CTCSpecialAbilities::CancelUsingAbility(EHeroAbility ability) -- cancel the hero ability in use: for the physical shield (if active), summon and ghost sword cancel the move and report true; for slow time, berserk, multi strike and multi arrow cancel the move and report false
// Leaf: ?CancelUsingAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z
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
        ~reference() { }
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
        if (UsingComponents[T::StatGetInterfaceType()]) {
            const TCMap* map = &Components;
            const TCMap::value_type* it = map->LowerBound(T::StatGetInterfaceType());
            if (it == map->Data.end() || T::StatGetInterfaceType() < it->first)
                it = map->Data.end();
            *out = (const T*)it->second;
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
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};
class CTCPhysicalShieldSpell : public CTCBase { public:                  // interface type 0x93
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x93; }
    bool IsShieldActive();                                  // 0x0079DCB0
    void CancelMove();                                      // 0x0079E300
};
class CTCSummonSpell : public CTCBase { public:                          // interface type 0x91
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x91; }
    void CancelMove();                                      // 0x0079A820
};
class CTCGhostSword : public CTCBase { public:                           // interface type 0x9A
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x9A; }
    void CancelMove();                                      // 0x007C5710
};
class CTCBulletTime : public CTCBase { public:                           // interface type 0x92
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x92; }
    void CancelMove();                                      // 0x0079C640
};
class CTCBerserk : public CTCBase { public:                              // interface type 0x84
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x84; }
    void CancelMove();                                      // 0x0078C610
};
class CTCMultiStrike : public CTCBase { public:                          // interface type 0x87
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x87; }
    void CancelMove();                                      // 0x007CC9B0
};
class CTCMultiArrow : public CTCBase { public:                           // interface type 0x88
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x88; }
    void CancelMove();                                      // 0x007CBEE0
};
class CTCSpecialAbilities : public CTCBase { public:      // interface type 0xA7
    class CUndeadFollower;
    bool                            AbilityMutexLock;       // +0x0C
    bool                            UsingBed;               // +0x0D
    EHeroAbility                    CurrentAbility;         // +0x10
    std::list<CUndeadFollower>      UndeadFollowers;        // +0x14
    CThingCreatureBase*             Creature;               // +0x18
    std::vector<bool>               AbilityAvailability;    // +0x1C (retail indexes it bytewise)
    bool CancelUsingAbility(EHeroAbility ability);
};

bool CTCSpecialAbilities::CancelUsingAbility(EHeroAbility ability)
{
    switch (ability) {
    case 4: {                                               // physical shield
        CTCPhysicalShieldSpell* shield;
        if (ParentThing->GetTC(&shield) && shield->IsShieldActive()) {
            shield->CancelMove();
            return true;
        }
        break;
    }
    case 10: {                                              // summon
        CTCSummonSpell* summon;
        if (ParentThing->GetTC(&summon)) {
            summon->CancelMove();
            return true;
        }
        break;
    }
    case 15: {                                              // ghost sword
        CTCGhostSword* sword;
        if (ParentThing->GetTC(&sword)) {
            sword->CancelMove();
            return true;
        }
        break;
    }
    case 2: {                                               // slow time (bullet time)
        CTCBulletTime* bulletTime;
        if (ParentThing->GetTC(&bulletTime))
            bulletTime->CancelMove();
        break;
    }
    case 1: case 3: case 5: case 6: case 7: case 8: case 11: {   // retail sends these to the berserk cancel (table 0x0072AF1C)
        CTCBerserk* berserk;
        if (ParentThing->GetTC(&berserk))
            berserk->CancelMove();
        break;
    }
    case 9: {                                               // multi strike
        CTCMultiStrike* multiStrike;
        if (ParentThing->GetTC(&multiStrike))
            multiStrike->CancelMove();
        break;
    }
    case 12: case 13: case 16: case 17: {                   // retail sends these to the multi-arrow cancel
        CTCMultiArrow* multiArrow;
        if (ParentThing->GetTC(&multiArrow))
            multiArrow->CancelMove();
        break;
    }
    }
    return false;
}