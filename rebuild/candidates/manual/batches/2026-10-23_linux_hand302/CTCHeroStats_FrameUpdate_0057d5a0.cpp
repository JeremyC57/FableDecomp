// CTCHeroStats::FrameUpdate() -- per-frame hero bookkeeping: region time, fatness decay, stamina and health refills, weapon augmentation regeneration, speed multiplier expiry and drunkenness
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057D5A0, 798 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed SetAge 0x0057B05F).
// Callees: CTCBase::GetWorldMap() @ 0x00686D30, CWorldMap::GetCurrentRegionNumber() @ 0x004FB150,
//          CTCHeroStats::UpdateAppearanceStats(bool) @ 0x0057D23B (descriptive name), CTCBase::GetDefinitions() @ 0x006869D0,
//          CTCHeroStats::SetFatness(float) @ 0x0057B169, CTCHeroStats::GetFatnessReducingFrequencyInFrames() @ 0x0057A60A,
//          std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020 / const @ 0x004365B0,
//          GFGetCurrentFrame() @ 0x0049D870, _ftol @ 0x00BFEA70, CThing virtual slot 59 (+0xEC, ModifyHealth(float, bool)),
//          CBaseIntelligentPointer::GetPItem() @ 0x00A01B50, IS_THING_ALIVE(CThing* const&) @ 0x00436380,
//          CTCObjectAugmentations::GetHealthToAddThisFrame() @ 0x00766260 / GetStaminaToAddThisFrame() @ 0x00766330,
//          CTCHaste::RemoveMultiplier(CTCHeroStats*) @ 0x0058148D, CTCHeroStats::ProcessDrunkenness() @ 0x0057CB7E.
// Globals: g_framesPerSecond @ 0x01375550.
// Shape: interfaces 0xA7 (a component whose +0xC byte blocks stamina refill), 0x93 (presence only), 0x13
// CTCInventoryWeapons (active melee +0x134 / ranged +0x141 weapon pointers, const pick), 0xBF CTCObjectAugmentations
// (const pick), 0x8C CTCHaste (out pointer zero-initialised). The const pick is `found = (it != last && !(k < it->type)) ? it
// : last` (the if-form puts found in ecx instead of the map register). The 0x93 presence test only runs when 0xA7 is
// present. IS_THING_ALIVE gets a same-TU read-only stand-in (it takes the weapon local's address): without it the
// interface key cannot share the melee weapon's frame word (frame 0x14 v 0x10).
// Layout: retail CTCHeroStats sits below the PDB (engine/CTCHeroStats.h): Fatness +0x38 (PDB +0x40), Stamina +0x58,
// MaxStamina +0x5C, FramesUntilStaminaRefills +0x60, FramesUntilHealthRefills +0x64, ScriptOverridePreventStaminaRefill
// +0x68, FramesUntilFatnessReduces +0x6C, FramesLeftForSpeedMultiplier +0x94 (PDB +0x9C), RegionTime +0x124 (PDB +0x130).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

struct CPresenceBits {
    bool test(long bit) const;                       // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void ModifyHealth(float amount, bool showEffect);   // slot 59 (+0xEC)
    unsigned char _pad_0x04[0x1C];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            const CInterfaceMap::SEntry* found = (it != map->last && !(wanted < it->type)) ? it : map->last;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};

bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x00436380
#pragma auto_inline(off)
// Same-TU stand-in for IS_THING_ALIVE 0x00436380 (not the retail body): it only reads the pointer.
bool __fastcall IS_THING_ALIVE(CThing* const& thing)
{
    return thing != 0 && thing->HasTC(0);
}
#pragma auto_inline(on)

struct CIntelligentPointer {
    unsigned char _body[8];
    CThing* GetPItem() const;                        // 0x00A01B50
};

#pragma pack(push, 1)
struct CTCInventoryWeapons {                         // interface 0x13
    unsigned char       _pad_0x00[0x134];
    CIntelligentPointer PActiveMeleeWeapon;          // +0x134
    long                ActiveMeleeWeaponDefIndex;   // +0x13C
    bool                MeleeWeaponCarriedOnLoad;    // +0x140
    CIntelligentPointer PActiveRangedWeapon;         // +0x141
};
#pragma pack(pop)

struct CTCObjectAugmentations {                      // interface 0xBF
    long GetHealthToAddThisFrame();                  // 0x00766260
    long GetStaminaToAddThisFrame();                 // 0x00766330
};

struct CStaminaRefillBlocker {                       // interface 0xA7
    unsigned char _pad_0x00[0xC];
    bool          BlockStaminaRefill;                // +0x0C
};

struct CTCHeroStats;
struct CTCHaste {                                    // interface 0x8C
    void RemoveMultiplier(CTCHeroStats* stats);      // 0x0058148D
};

struct CWorldMap {
    long GetCurrentRegionNumber();                   // 0x004FB150
};

struct CLongVector {                                 // std::vector<long> (retail 12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    long& operator[](unsigned int n) { return _M_start[n]; }
};

struct CHeroStatsDef {
    unsigned char _pad_0x00[0x48];
    float         SecondsBetweenHealthRefills;       // +0x48
    long          StaminaRefillAmount;               // +0x4C
    long          HealthRefillAmount;                // +0x50
    unsigned char _pad_0x54[0x30];
    float         FatnessReduction;                  // +0x84
};

struct CDefinitionHolder {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC CDefPointer<const CHeroStatsDef>::Object
};

long __fastcall GFGetCurrentFrame();                 // 0x0049D870
extern long g_framesPerSecond;                       // 0x01375550

struct CTCHeroStats {
    void*         __vftable;                         // +0x000
    CThing*       ParentThing;                       // +0x004 CTCBase
    unsigned char _pad_0x008[0x30];
    float         Fatness;                           // +0x038 (PDB +0x40)
    unsigned char _pad_0x03c[0x1C];
    long          Stamina;                           // +0x058 (PDB +0x60)
    long          MaxStamina;                        // +0x05C
    long          FramesUntilStaminaRefills;         // +0x060
    long          FramesUntilHealthRefills;          // +0x064
    bool          ScriptOverridePreventStaminaRefill;   // +0x068
    long          FramesUntilFatnessReduces;         // +0x06C (PDB +0x74)
    unsigned char _pad_0x070[0x24];
    long          FramesLeftForSpeedMultiplier;      // +0x094 (PDB +0x9C)
    unsigned char _pad_0x098[0x8C];
    CLongVector   RegionTime;                        // +0x124 (PDB +0x130)

    CWorldMap* GetWorldMap();                        // 0x00686D30
    CDefinitionHolder* GetDefinitions() const;       // 0x006869D0
    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitions()->PHeroStatsDef; }
    void UpdateAppearanceStats(bool force);          // 0x0057D23B
    void SetFatness(float fatness);                  // 0x0057B169
    long GetFatnessReducingFrequencyInFrames() const;   // 0x0057A60A
    void ProcessDrunkenness();                       // 0x0057CB7E
    void FrameUpdate();
};

void CTCHeroStats::FrameUpdate()
{
    long region = GetWorldMap()->GetCurrentRegionNumber();
    if (region >= 0 && region < RegionTime.size())
        ++RegionTime[region];
    UpdateAppearanceStats(false);
    --FramesUntilFatnessReduces;
    if (FramesUntilFatnessReduces <= 0) {
        SetFatness(Fatness - GetHeroStatsDef()->FatnessReduction);
        FramesUntilFatnessReduces = GetFatnessReducingFrequencyInFrames();
    }
    if (!ScriptOverridePreventStaminaRefill) {
        CStaminaRefillBlocker* blocker;
        if (!(ParentThing->GetInterface(&blocker, 0xA7) && (blocker->BlockStaminaRefill || ParentThing->HasTC(0x93)))) {
            --FramesUntilStaminaRefills;
            if (FramesUntilStaminaRefills <= 0) {
                long fps = g_framesPerSecond;
                if (GFGetCurrentFrame() % fps == 0)
                    Stamina += GetHeroStatsDef()->StaminaRefillAmount;
                if (Stamina < 0)
                    Stamina = 0;
                else if (Stamina > MaxStamina)
                    Stamina = MaxStamina;
                FramesUntilStaminaRefills = 0;
            }
        }
    }
    CThing* thing = ParentThing;
    --FramesUntilHealthRefills;
    if (FramesUntilHealthRefills <= 0) {
        long amount = GetHeroStatsDef()->HealthRefillAmount;
        if (amount != 0)
            thing->ModifyHealth((float)amount, true);
        FramesUntilHealthRefills = (long)(g_framesPerSecond * GetHeroStatsDef()->SecondsBetweenHealthRefills);
    }
    CTCInventoryWeapons* weapons;
    if (ParentThing->PeekInterface(&weapons, 0x13)) {
        CThing* melee = weapons->PActiveMeleeWeapon.GetPItem();
        CThing* ranged = weapons->PActiveRangedWeapon.GetPItem();
        CTCObjectAugmentations* augmentations;
        if (IS_THING_ALIVE(melee) && melee->PeekInterface(&augmentations, 0xBF)) {
            thing->ModifyHealth((float)augmentations->GetHealthToAddThisFrame(), false);
            if (!ScriptOverridePreventStaminaRefill)
                Stamina += augmentations->GetStaminaToAddThisFrame();
        }
        if (IS_THING_ALIVE(ranged) && ranged->PeekInterface(&augmentations, 0xBF)) {
            thing->ModifyHealth((float)augmentations->GetHealthToAddThisFrame(), false);
            if (!ScriptOverridePreventStaminaRefill)
                Stamina += augmentations->GetStaminaToAddThisFrame();
        }
        if (Stamina < 0)
            Stamina = 0;
        else if (Stamina > MaxStamina)
            Stamina = MaxStamina;
    }
    long& speedFrames = FramesLeftForSpeedMultiplier;
    if (speedFrames > 0 && --speedFrames <= 0) {
        CTCHaste* haste = 0;
        ParentThing->GetInterface(&haste, 0x8C);
        haste->RemoveMultiplier(this);
    }
    ProcessDrunkenness();
}
