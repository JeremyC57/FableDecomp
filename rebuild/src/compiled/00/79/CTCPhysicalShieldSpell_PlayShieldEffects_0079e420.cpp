// CTCPhysicalShieldSpell::PlayShieldEffects(CThingCreatureBase* creature) -- advance the shield wobble (bouncing between 0 and 1 at the def's wobble rate) and highlight the creature's appearance with a width between the def's minimum and the level's maximum
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079E420, 211 bytes (`ret 4`; PDB ?PlayShieldEffects@CTCPhysicalShieldSpell@@IAEXPAVCThingCreatureBase@@@Z).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, NGraphicAppearance::CTCGraphicAppearance::
//   SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) @ 0x004C04C0.
//   Float pool: 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC. Interface 0x5b = CTCGraphicAppearance (bitset word 2 bit 27).
// Shape: the appearance comes from the out-parameter GetInterface whose entry pick is one end/hit ternary
//   (`*out = (it == last || k < it->type ? last : it)->component`): the component loads straight into ecx before the
//   argument pushes and the bool goes through eax. The if-form pick (or a HasTC test + GetTC) loads it after the pushes.
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
template <class T> struct vector {              // retail STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CDefString { long m_index; };
struct CSpecialAbilitiesPhysicalShieldDef {     // retail layout: PDB offset - 4, 12-byte vectors
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
template <class T> struct CDefPointer {
    const T* Object;
    const T* operator->() const { return Object; }
};
struct CRGBColour { unsigned char B, G, R, A; };
struct CIntelligentPointer {                    // CIntelligentPointer<CThing> (8 bytes)
    void* __vftable;
    void* m_pItem;
};
struct CTCHeroStats {
    unsigned char _pad00[0x58];
    long Stamina;                               // +0x58 (PDB +0x60; retail CTCHeroStats sits 8 bytes below the PDB)
    void ModifyStaminaBy(long amount);          // 0x0057B1DB
};
namespace NGraphicAppearance {
    enum EAppearanceMeshEffectPriority { PRIORITY_4 = 4 };
    struct CTCGraphicAppearance {
        void SetAsHighlighted(float start, float width, const CRGBColour& colour, bool includeCarried, bool flag,
                              EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
    };
}
struct CTCPhysicalShieldSpell {
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

void CTCPhysicalShieldSpell::PlayShieldEffects(CThingCreatureBase* creature)
{
    CurWobbleTimer += WobbleDelta;
    if (CurWobbleTimer > 1.0f) {
        CurWobbleTimer = 1.0f;
        WobbleDelta = -PDef->WobbleRate;
    } else if (CurWobbleTimer < 0.0f) {
        CurWobbleTimer = 0.0f;
        WobbleDelta = PDef->WobbleRate;
    }
    const CSpecialAbilitiesPhysicalShieldDef* def = PDef.Object;
    float width = (def->MaxWidth[SpellLevel] - def->MinWidth) * CurWobbleTimer + def->MinWidth;
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    if (creature->GetInterface(0x5b, &appearance)) {
        appearance->SetAsHighlighted(0.0f, width, ShieldColor, def->ShieldCoversCarriedItems, true,
                                     NGraphicAppearance::PRIORITY_4, this);
    }
}