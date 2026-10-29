// CTCPhysicalShieldSpell::TakeDamage(float damage) -- while the shield runs untimed, a hit drains the hero's stamina (or, without hero stats, the shield's own health) by damage * the level's mana-damage factor; running dry ends the shield
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079E250, 166 bytes (`ret 4`; PDB ?TakeDamage@CTCPhysicalShieldSpell@@QAEXM@Z).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCHeroStats::ModifyStaminaBy(long) @ 0x0057B1DB,
//   __ftol2 @ 0x00BFEA70. Float pool: 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCPhysicalShieldSpell (PDef +0x0c, SpellLevel +0x10, DurationTimer +0x1c, ShieldHealth +0x20, as the landed
//   Pause 0x0079E3A0); CSpecialAbilitiesPhysicalShieldDef at PDB offset - 4 with 12-byte STLport vectors (ManaDamage +0x138);
//   CTCHeroStats Stamina +0x58 (PDB +0x60, retail 8 bytes below). Interface type 4 = CTCHeroStats (the ModifyStaminaBy callee).
// Shape: the thing's out-parameter GetInterface (bitset test `test byte ptr [eax+20h],10h`, the key in the `push ecx` frame
//   word, `it == last || key < it->type` -> last, no null test of the component) must be `__forceinline` in this local model or
//   it is emitted out of line; the component lands in esi because it is used again after the call. `x <= 0.0f` on the stored
//   health is `fst ; fcomp ; test ah,41h ; jp`.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->first) ? map->last->second : it->second;
    }
};
class CThingCreatureBase : public CThing { public: };
template <class T> struct vector {              // retail STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CDefString { public: long m_index; };
class CSpecialAbilitiesPhysicalShieldDef { public:     // retail layout: PDB offset - 4, 12-byte vectors
    unsigned char _pad00[0x3c];                 // CDefClassBase body
    CDefString InitSound;                       // +0x3c
    CDefString PowerUpSound[3];                 // +0x40
    CDefString CastSound;                       // +0x4c
    CDefString LoopingSound[4];                 // +0x50
    CDefString EndSound;                        // +0x60
    float Health[4];                            // +0x64
    float LevelUpTimings[3];                    // +0x74
    float CamDist[4];                           // +0x80
    float CamHeight[4];                         // +0x90
    float CamFOV[4];                            // +0xa0
    float CamSwitchTime[4];                     // +0xb0
    float CastCamDist;                          // +0xc0
    float CastCamHeight;                        // +0xc4
    float CastCamFOV;                           // +0xc8
    float CastCamSwitchTime;                    // +0xcc
    float MinWidth;                             // +0xd0
    vector<float> MaxWidth;                     // +0xd4
    float BeenHitExtraWidth;                    // +0xe0
    float BeenHitFadeOffTimeSecs;               // +0xe4
    float WobbleRate;                           // +0xe8
    vector<float> ColorStartLevel1;             // +0xec
    vector<float> ColorStartLevel2;             // +0xf8
    vector<float> ColorStartLevel3;             // +0x104
    vector<float> ColorStartLevel4;             // +0x110
    vector<float> ColorEnd;                     // +0x11c
    vector<float> ColorHit;                     // +0x128
    float FadeOutPhysicalShieldTimeSecs;        // +0x134
    vector<float> ManaDamage;                   // +0x138
    bool  ShieldCoversCarriedItems;             // +0x144
};
template <class T> class CDefPointer { public:
    const T* Object;
    const T* operator->() const { return Object; }
};
class CRGBColour { public: unsigned char B, G, R, A; };
struct CIntelligentPointer {                    // CIntelligentPointer<CThing> (8 bytes)
    void* __vftable;
    void* m_pItem;
};
class CTCHeroStats : public CTCHeroStatsDonor {
 public:
    void ModifyStaminaBy(long amount);          // 0x0057B1DB
};
namespace NGraphicAppearance {
    enum EAppearanceMeshEffectPriority { PRIORITY_4 = 4 };
    class CTCGraphicAppearance { public:
        void SetAsHighlighted(float start, float width, const CRGBColour& colour, bool includeCarried, bool flag,
                              EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
    };
}
class CTCPhysicalShieldSpell { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    CDefPointer<CSpecialAbilitiesPhysicalShieldDef> PDef;   // +0x0c
    long    SpellLevel;                         // +0x10
    long    ShieldFadeOutTimer;                 // +0x14
    long    CurShieldFadeOutTimer;              // +0x18
    long    DurationTimer;                      // +0x1c
    float   ShieldHealth;                       // +0x20
    float   CurWobbleTimer;                     // +0x24
    float   WobbleDelta;                        // +0x28
    CRGBColour ShieldColor;                     // +0x2c
    CIntelligentPointer ShieldEffect;           // +0x30
    CIntelligentPointer SwirlyEffect;           // +0x38
    unsigned long LoopingSound;                 // +0x40
    bool    Pausable;                           // +0x44
    bool    Paused;                             // +0x45
    bool    CutSceneCancelled;                  // +0x46
    void TakeDamage(float damage);
    void PlayShieldEffects(CThingCreatureBase* creature);
};

void CTCPhysicalShieldSpell::TakeDamage(float damage)
{
    if (DurationTimer == -1) {
        CTCHeroStats* stats;
        if (ParentThing->GetInterface(4, &stats)) {
            stats->ModifyStaminaBy((long)(damage * PDef->ManaDamage[SpellLevel]));
            if (stats->Stamina <= 0)
                DurationTimer = 0;
        } else {
            ShieldHealth += damage * PDef->ManaDamage[SpellLevel];
            if (ShieldHealth <= 0.0f)
                DurationTimer = 0;
        }
    }
}