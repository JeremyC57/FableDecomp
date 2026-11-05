// CCreatureAction_UnsheatheWeapons::PerformAction() -- carry out the (un)sheathe the creature's combat component has pending: sheathe all, unsheathe all / default / melee / ranged by the pending type; when the creature then carries a weapon, refresh its current creature mode and make the SIMPLE_NOISE noise; set up the strike events and clear the pending type
// Leaf: ?PerformAction@CCreatureAction_UnsheatheWeapons@@UAEXXZ
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B7BA0, 315 bytes of code + `nop` + the 5-entry jump table (row 336), plain `ret`.
// Identity: Ego_r.pdb `?PerformAction@CCreatureAction_UnsheatheWeapons@@UAEXXZ`; the catalog label is right.
//   Jump table 0x008B7CDC checked entry by entry: cases 1..5 -> 0x008B7BFE SheatheAllWeapons, 0x008B7C0E UnsheatheAllWeapons,
//   0x008B7C1E UnsheatheDefaultWeapon, 0x008B7C2E UnsheatheMeleeWeapon, 0x008B7C3E UnsheatheRangedWeapon (as the landed
//   ShouldPlayUnsheatheWeaponAction 0x008B7CF0 maps the types: default 2/3, melee 4, ranged 5).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, each use), CVectorMap<ETCInterfaceType,CTCBase*>::
//   LowerBound 0x0040F020 (inline CThing::GetTC<T>(T**)), CThingCreatureBase::SheatheAllWeapons 0x006647F0 /
//   UnsheatheAllWeapons 0x00666830 / UnsheatheDefaultWeapon 0x00664A50 / UnsheatheMeleeWeapon 0x006649C0 /
//   UnsheatheRangedWeapon 0x00664A10 (all Ego_r.pdb QAEXXZ), CTCCarrying::IsThingCarryingWeapon(const CThing&) 0x006A1540
//   (Ego_r.pdb static `SI_NABVCThing@@@Z`; the body tests CTCCarrying 0x46 and the primary-slot thing's CTCWeapon bit),
//   CTCCreatureModeManager::RefreshCurrentMode 0x0073A840 (Ego_r.pdb QAEXXZ; donor order GetCurrentMode / RefreshCurrentMode
//   / FrameUpdate / IsModeActive = retail 0x0073A840 / 0x0073A880 (front mode slot 3) / 0x0073A8A0), CCharString(const char*,
//   long) 0x0099EBF0 (literal 0x01268A5C "SIMPLE_NOISE") / ~CCharString 0x0099EAE0, CThingCreatureBase::MakeNoise(const
//   CCharString&) 0x00662110, CCombatActionBase::SetupCombatStrikeEvents 0x00859A70 (Ego_r.pdb protected IAEXXZ; the landed
//   body builds the strike events).
// Interface types (vtable slot 21): CTCCombat 0x30 (0x00739CE0), CTCCreatureModeManager 0x31 (0x00744270).
// Layout: CTCCombat UnsheathingWeaponType +0x6c (Ego_r +0x78 less three 4-byte-narrower vectors; the landed Initialise
//   0x008B7ED0 stores it there); CThing UsingComponents +0x20, Components +0x44; CCreatureActionBase PCreature +0x08.
// Shape: the pending type 0 returns before the switch (`test ; je` to the shared pops), cases 1..5 through the table; the
//   noise string is block-scoped, so it shares the one `push ecx` frame word with both lookup keys.
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"

enum ETCInterfaceType { };
class CTCBase;
class CCharString { public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CThing { public:                                      // partial view, retail offsets
    virtual ~CThing();
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      _pad_0x04[0x20 - 0x04];
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound((ETCInterfaceType)T::INTERFACE_TYPE);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public: };
class CThingGameObject : public CThingPhysical { public: };
class CThingCreatureBase : public CThingGameObject { public:
    void SheatheAllWeapons();                               // 0x006647F0
    void UnsheatheAllWeapons();                             // 0x00666830
    void UnsheatheDefaultWeapon();                          // 0x00664A50
    void UnsheatheMeleeWeapon();                            // 0x006649C0
    void UnsheatheRangedWeapon();                           // 0x00664A10
    void MakeNoise(const CCharString& noise);               // 0x00662110
};

class CBaseObjectPointer;
class CBaseObject;
#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* operator->() const { return GetPointer(); }
    T& operator*() const { return *GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

class CTCBase { public:
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};
enum EUnsheatheWeaponType {                                 // enumerators descriptive (not in the PDB dumps)
    UNSHEATHE_WEAPON_TYPE_0,
    UNSHEATHE_WEAPON_TYPE_1,
    UNSHEATHE_WEAPON_TYPE_2,
    UNSHEATHE_WEAPON_TYPE_3,
    UNSHEATHE_WEAPON_TYPE_4,
    UNSHEATHE_WEAPON_TYPE_5
};
class CTCCombat : public CTCBase { public:                  // interface type 0x30
    enum { INTERFACE_TYPE = 0x30 };
    unsigned char        _pad_0x0c[0x6c - 0x0c];
    EUnsheatheWeaponType UnsheathingWeaponType;             // +0x6c
};
class CTCCreatureModeManager : public CTCBase { public:     // interface type 0x31
    enum { INTERFACE_TYPE = 0x31 };
    void RefreshCurrentMode();                              // 0x0073A840
};
class CTCCarrying : public CTCBase { public:
    static bool __fastcall IsThingCarryingWeapon(const CThing& thing);   // 0x006A1540
};

class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
    unsigned char                            _pad_0x04[0x04];
    CIntelligentPointer<CThingCreatureBase>  PCreature;     // +0x08
    virtual void PerformAction();
};
class CCombatActionBase : public CCreatureActionBase { public:
protected:
    void SetupCombatStrikeEvents();                         // 0x00859A70
};
class CCreatureAction_UnsheatheWeapons : public CCombatActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_UnsheatheWeapons::PerformAction()
{
    CTCCombat* combat;
    if (PCreature->GetTC(&combat)) {
        if (combat->UnsheathingWeaponType == UNSHEATHE_WEAPON_TYPE_0)
            return;
        switch (combat->UnsheathingWeaponType) {
        case UNSHEATHE_WEAPON_TYPE_1:
            PCreature->SheatheAllWeapons();
            break;
        case UNSHEATHE_WEAPON_TYPE_2:
            PCreature->UnsheatheAllWeapons();
            break;
        case UNSHEATHE_WEAPON_TYPE_3:
            PCreature->UnsheatheDefaultWeapon();
            break;
        case UNSHEATHE_WEAPON_TYPE_4:
            PCreature->UnsheatheMeleeWeapon();
            break;
        case UNSHEATHE_WEAPON_TYPE_5:
            PCreature->UnsheatheRangedWeapon();
            break;
        }
        if (CTCCarrying::IsThingCarryingWeapon(*PCreature)) {
            CTCCreatureModeManager* modes;
            if (PCreature->GetTC(&modes))
                modes->RefreshCurrentMode();
            {
                CCharString noise("SIMPLE_NOISE", -1);
                PCreature->MakeNoise(noise);
            }
        }
        SetupCombatStrikeEvents();
        combat->UnsheathingWeaponType = UNSHEATHE_WEAPON_TYPE_0;
    }
}