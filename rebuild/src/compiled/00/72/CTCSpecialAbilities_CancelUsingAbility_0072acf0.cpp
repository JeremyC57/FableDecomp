// CTCSpecialAbilities::CancelUsingAbility(EHeroAbility ability) -- cancel the hero ability in use: for the physical shield (if active), summon and ghost sword cancel the move and report true; for slow time, berserk, multi strike and multi arrow cancel the move and report false
// Leaf: ?CancelUsingAbility@CTCSpecialAbilities@@QAE_NW4EHeroAbility@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"


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
            if (it == map->end() || T::StatGetInterfaceType() < it->first)
                it = map->end();
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
            if (it == map->end() || T::StatGetInterfaceType() < it->first)
                it = map->end();
            *out = (const T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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