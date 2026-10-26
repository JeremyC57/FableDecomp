// CHeroCombatDef::~CHeroCombatDef() -- destroy the members in reverse (the heavy-weapons stat package, the two POD vectors, the seven stat-weight maps), then the CParentDefClassBase part
// Leaf: ??1CHeroCombatDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458B9D, length 124 bytes. Size-optimised def module (`#pragma optimize("s", on)`:
// `pop ecx` after each free).
// Label correction: catalogued as CTCAssassinRush::FinishMove (propagated). The only caller is 0x00458B84 inside the deleting
// destructor 0x00458B81, which sits right after CHeroCombatDef's slot-20 Sizeof 0x00458B7B (`mov eax,148h`, vtable 0x01234CF4);
// no vtable store of its own (def destructors run with the base's).
// Callees: map<EHeroTrainableStatType,long>::~map @ 0x00451763 (HeavyWeapons' StatThresholds), free @ 0x00BFEA14 (the inline
// vector<float>/vector<long> dtors), map<EHeroTrainableStatType,float>::~map @ 0x004517A4 (x7; catalog labels for both maps are
// the propagated "CActiveFile::OnReadFinished"), ~CParentDefClassBase @ 0x00430300 (tail jump; catalog "CTCInventoryItem::OnDie").
// Layout: Ego_r.pdb CHeroCombatDef (0x174) with retail containers: STLport maps 12 bytes (PDB 16), vectors 12 (PDB 16), the
// PDB members 4 bytes lower from the start (retail 0x148, slot 20): stat weights at +0x54/+0x68/+0x78/+0x9C/+0xB0/+0xC4/+0xF8,
// DefaultCombatAbilities +0x11C, ProjectileWeaponFirstPersonModeMaxFOVValues +0x128, HeavyWeapons +0x13C.
// Shape: as the landed def dtors (0x0045784B): empty body, `__forceinline` POD-vector dtors, out-of-line map dtors.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* memory);        // 0x00BFEA14
class CParentDefClassBase { public:                       // retail 0x3c bytes (with the CDefClassBase part)
    unsigned char m_baseData[0x3c];
    ~CParentDefClassBase();                        // 0x00430300
};
template <class T> struct CPODVector {             // retail std::vector<T> (12 bytes)
    T* m_first;
    T* m_last;
    T* m_end;
    __forceinline ~CPODVector() { if (m_first) free(m_first); }
};
class CFloatRange { public: float Min, Max; };
class CHeroAbilityStatWeight { public:                    // map<EHeroTrainableStatType, float> (STLport, 12 bytes)
    unsigned char StatWeights[12];
    ~CHeroAbilityStatWeight();                     // 0x004517A4
};
class CHeroAbilityStatPackage { public:                   // map<EHeroTrainableStatType, long> (STLport, 12 bytes)
    unsigned char StatThresholds[12];
    ~CHeroAbilityStatPackage();                    // 0x00451763
};

class CHeroCombatDef : public CParentDefClassBase { public:      // vtable 0x01234CF4, retail 0x148 bytes
    CFloatRange RangedAccuracySecondsForMaximumWithSkill;      // +0x3c
    CFloatRange RangedAccuracyPercentFractionWithBuildup;      // +0x44
    CFloatRange RangedAccuracyPercentWithStats;                // +0x4c
    CHeroAbilityStatWeight RangedAccuracyWeighting;            // +0x54
    CFloatRange RangedAccuracyShakeyCursor;                    // +0x60
    CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting;    // +0x68
    float RangedAccuracyShakeFractionOfRadius;                 // +0x74
    CHeroAbilityStatWeight RangedDamageMultiplierWeighting;    // +0x78
    float RangedDamageMultiplierSpeedWithStatParameter;        // +0x84
    float RangedDamageMultiplierSpeedWithDistance;             // +0x88
    float RangedDamageMultiplierSpeedNominalBuildup;           // +0x8c
    float RangedDamageMultiplierScaleNominalBuildup;           // +0x90
    float RangedDamageMultiplierSpeedSuperBuildup;             // +0x94
    float RangedDamageMultiplierScaleSuperBuildup;             // +0x98
    CHeroAbilityStatWeight RangedWeaponReloadSpeedWeighting;   // +0x9c
    CFloatRange MeleeDamageMultiplierWithStats;                // +0xa8
    CHeroAbilityStatWeight MeleeDamageWeighting;               // +0xb0
    CFloatRange HitDamageMultiplierWithStats;                  // +0xbc
    CHeroAbilityStatWeight HitDamageWeighting;                 // +0xc4
    bool MeleeFlourishValidOnMultiplierLevelIncrease;          // +0xd0
    long MeleeFlourishNumMultiplierLevelsPerFlourish;          // +0xd4
    bool MeleeFlourishAllowBreakingOfRotation;                 // +0xd8
    float MeleeFlourishSecondsForAvailibility;                 // +0xdc
    bool MeleeFlourishOneShot;                                 // +0xe0
    bool MeleeFlourishUsable;                                  // +0xe1
    bool MeleeCombatPlayStrikeSounds;                          // +0xe2
    float TargetingPressureFractionThresholdForToggle;         // +0xe4
    float MaxAngleToTurnToShootAtTargetInThirdPersonBowMode;   // +0xe8
    long MeleeStrikeNumFramesPauseOnHeroStrike;                // +0xec
    CFloatRange MeleeStrikeAnimationSpeedFractionWithStats;    // +0xf0
    CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting; // +0xf8
    float AutoBlockSkillThreshold;                             // +0x104
    float AutoBlockStrengthThreshold;                          // +0x108
    float MeleeRecoilMaintainHandednessSkillThreshold;         // +0x10c
    float MeleeRecoilMaintainHandednessStrengthThreshold;      // +0x110
    float MeleeRecoilMaintainComboChainSkillThreshold;         // +0x114
    float MeleeRecoilMaintainComboChainStrengthThreshold;      // +0x118
    CPODVector<long> DefaultCombatAbilities;                   // +0x11c
    CPODVector<float> ProjectileWeaponFirstPersonModeMaxFOVValues; // +0x128
    bool ProjectileTargetingAcquireTargetTag;                  // +0x134
    bool ProjectileTargetingAcquireTargetsAuto;                // +0x135
    bool ProjectileTargetingUseAnalogueZoom;                   // +0x136
    float ProjectileWeaponAnalogueZoomControlWeight;           // +0x138
    CHeroAbilityStatPackage HeavyWeapons;                      // +0x13c
    ~CHeroCombatDef();
};

CHeroCombatDef::~CHeroCombatDef()
{
}