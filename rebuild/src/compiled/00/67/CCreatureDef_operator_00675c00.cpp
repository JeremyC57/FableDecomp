// CCreatureDef::operator=(const CCreatureDef&) -- self-checked def assignment: deep-copies the four counted-pointer payloads (wound morphs, random appearance morph, expression set, creature stats), then the action/appearance vectors, phoneme map, dialogue voices, the inventory/screen defs, respawn timings, flags, radius weight/spread; returns *this
// Leaf: ??4CCreatureDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675C00, length 544 bytes.
// Callees: vector<CWoundMorphs::CEntry>::operator= @ 0x00674520, CRandomAppearanceMorph::operator=
//          @ 0x00675B60, vector<CExpressionSet::CEntry>::operator= @ 0x00674400, CDefClassBase::operator=
//          @ 0x00431F10, vector<long>::operator= @ 0x00454886 (x2), map<CCharString,long>::operator=
//          @ 0x00432B38, vector<CDefString>::operator= @ 0x0046078C, CHeroAbilityStatWeight::operator=
//          @ 0x0046230E.
// Identity: reached through the jmp thunk 0x00676A50 in slot 19 of vtable 0x0125B43C = CCreatureDef
// (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated.
// Layout: PDB CCreatureDef (Ego_r) uses 16-byte containers; retail STLport containers are 12 bytes,
// so InitialActions/InitialAppearanceModifiers/PhonemeAnim/DialogueVoices sit at +0x48/+0x54/+0x60/
// +0x6c and every later member 0x10 lower (Inventory +0x78 .. AGhostIsWhatIBe +0xe4).
// The payload types are CVectorMap wrappers: the 12-byte entry vector, the empty key compare at +0xc
// (nothing to copy) and the sorted flag at +0xd -- the inlined generated assignment is the vector
// call plus one byte copy. CRandomAppearanceMorph's assignment is out of line.
// Shape: user operator= under `&other != this` (gives `cmp edi,esi`), assigning the inventory/screen block in the order
// retail stores it (+0x98..+0xa4 before +0x94/+0x90) and UseActualHitPosForHitEffects ahead of the
// other bools.

struct CWoundMorphEntryVector {             // retail std::vector<CWoundMorphs::CEntry> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CWoundMorphEntryVector& operator=(const CWoundMorphEntryVector& other);   // 0x00674520
};
struct CExpressionEntryVector {             // retail std::vector<CExpressionSet::CEntry> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CExpressionEntryVector& operator=(const CExpressionEntryVector& other);   // 0x00674400
};
struct CKeyPairCompareLess {};              // empty key compare

struct CWoundMorphMap {                     // CVectorMap<EBodyLocation, CWoundMorphs::CEntry, ...>
    CWoundMorphEntryVector m_entries;       // +0x0
    CKeyPairCompareLess m_compare;          // +0xc
    bool m_sorted;                          // +0xd
};
struct CExpressionMap {                     // CVectorMap<EExpressionType, CExpressionSet::CEntry, ...>
    CExpressionEntryVector m_entries;       // +0x0
    CKeyPairCompareLess m_compare;          // +0xc
    bool m_sorted;                          // +0xd
};
class CWoundMorphs { public: CWoundMorphMap Morphs; };
class CExpressionSet { public: CExpressionMap Expressions; };
class CRandomAppearanceMorph { public:
    unsigned char _body[0x40];
    CRandomAppearanceMorph& operator=(const CRandomAppearanceMorph& other);   // 0x00675B60
};

class CDefClassBase { public:
    unsigned char _body[0x28];              // vptr, resource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CCreatureStatsDef : public CDefClassBase { public:
    long MoralityForKill;                   // +0x28
    long MoralityForPunch;                  // +0x2c
    long MoralityForWeaponHit;              // +0x30
    long ExperienceWorth;                   // +0x34
    long RenownWorth;                       // +0x38
    float Speed;                            // +0x3c
    float Dexterity;                        // +0x40
    float StrengthDamageMultiplier;         // +0x44
};

template <class T> class CCountedPointer { public:  // object pointer + shared count (8 bytes)
    T* m_pObject;
    long* m_pCount;
    T& operator*() const { return *m_pObject; }
};

struct CLongVector {                        // retail std::vector<long> (12 bytes)
    long* m_first; long* m_last; long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};
struct CPhonemeAnimMap {                    // retail std::map<CCharString,long> (12 bytes)
    void* m_head; unsigned long m_size; unsigned long m_compare;
    CPhonemeAnimMap& operator=(const CPhonemeAnimMap& other);   // 0x00432B38
};
struct CDefStringVector {                   // retail std::vector<CDefString> (12 bytes)
    void* m_first; void* m_last; void* m_end;
    CDefStringVector& operator=(const CDefStringVector& other);   // 0x0046078C
};
class CDefString { public: long Index; };
class CHeroAbilityStatWeight { public:             // map<EHeroTrainableStatType,float> StatWeights (12 bytes)
    void* m_head; unsigned long m_size; unsigned long m_compare;
    CHeroAbilityStatWeight& operator=(const CHeroAbilityStatWeight& other);   // 0x0046230E
};
class CFloatRange { public: float Min; float Max; };

class CCreatureDef : public CDefClassBase { public:                                // vtable 0x0125B43C
    CCountedPointer<CWoundMorphs> PWoundMorphs;                     // +0x28
    CCountedPointer<CRandomAppearanceMorph> PRandomAppearanceMorph; // +0x30
    CCountedPointer<CExpressionSet> PExpressions;                   // +0x38
    CCountedPointer<CCreatureStatsDef> PStats;                      // +0x40
    CLongVector InitialActions;                 // +0x48
    CLongVector InitialAppearanceModifiers;     // +0x54
    CPhonemeAnimMap PhonemeAnim;                // +0x60
    CDefStringVector DialogueVoices;            // +0x6c
    long Inventory;                             // +0x78
    long ClothingInventory;                     // +0x7c
    long WeaponsInventory;                      // +0x80
    long HeroAbilitiesScreenInventory;          // +0x84
    long QuestCardScreenInventory;              // +0x88
    long MapScreenInventory;                    // +0x8c
    long StatsScreenInventory;                  // +0x90
    long MagicScreenInventory;                  // +0x94
    long ExperienceScreenInventory;             // +0x98
    long TradeScreenInventory;                  // +0x9c
    long AugmentScreenInventory;                // +0xa0
    long QuestsScreenInventory;                 // +0xa4
    long InGameMenuScreen;                      // +0xa8
    unsigned long CreatureGroup;                // +0xac
    CDefString BattleCrySound;                  // +0xb0
    long OpinionSourceDef;                      // +0xb4
    long RespawnWaitInDaysMin;                  // +0xb8
    long RespawnWaitInDaysMax;                  // +0xbc
    long RespawnWaitInFramesMin;                // +0xc0
    long RespawnWaitInFramesMax;                // +0xc4
    bool Short;                                 // +0xc8
    bool FlashOnHit;                            // +0xc9
    bool PickPocketable;                        // +0xca
    bool ChildThatCanRegionFollow;              // +0xcb
    bool AllowedInProtectedTowns;               // +0xcc
    bool UseActualHitPosForHitEffects;          // +0xcd
    CHeroAbilityStatWeight RadiusWeight;        // +0xd0
    CFloatRange RadiusSpread;                   // +0xdc
    bool AGhostIsWhatIBe;                       // +0xe4
    CCreatureDef& operator=(const CCreatureDef& other);
};

CCreatureDef& CCreatureDef::operator=(const CCreatureDef& other)
{
    if (&other != this)
    {
        *PWoundMorphs = *other.PWoundMorphs;
        *PRandomAppearanceMorph = *other.PRandomAppearanceMorph;
        *PExpressions = *other.PExpressions;
        *PStats = *other.PStats;
        InitialActions = other.InitialActions;
        InitialAppearanceModifiers = other.InitialAppearanceModifiers;
        PhonemeAnim = other.PhonemeAnim;
        DialogueVoices = other.DialogueVoices;
        Inventory = other.Inventory;
        ClothingInventory = other.ClothingInventory;
        WeaponsInventory = other.WeaponsInventory;
        HeroAbilitiesScreenInventory = other.HeroAbilitiesScreenInventory;
        QuestCardScreenInventory = other.QuestCardScreenInventory;
        MapScreenInventory = other.MapScreenInventory;
        ExperienceScreenInventory = other.ExperienceScreenInventory;
        TradeScreenInventory = other.TradeScreenInventory;
        AugmentScreenInventory = other.AugmentScreenInventory;
        QuestsScreenInventory = other.QuestsScreenInventory;
        MagicScreenInventory = other.MagicScreenInventory;
        StatsScreenInventory = other.StatsScreenInventory;
        InGameMenuScreen = other.InGameMenuScreen;
        CreatureGroup = other.CreatureGroup;
        BattleCrySound = other.BattleCrySound;
        OpinionSourceDef = other.OpinionSourceDef;
        RespawnWaitInDaysMin = other.RespawnWaitInDaysMin;
        RespawnWaitInDaysMax = other.RespawnWaitInDaysMax;
        RespawnWaitInFramesMin = other.RespawnWaitInFramesMin;
        RespawnWaitInFramesMax = other.RespawnWaitInFramesMax;
        UseActualHitPosForHitEffects = other.UseActualHitPosForHitEffects;
        Short = other.Short;
        FlashOnHit = other.FlashOnHit;
        PickPocketable = other.PickPocketable;
        ChildThatCanRegionFollow = other.ChildThatCanRegionFollow;
        AllowedInProtectedTowns = other.AllowedInProtectedTowns;
        RadiusWeight = other.RadiusWeight;
        RadiusSpread = other.RadiusSpread;
        AGhostIsWhatIBe = other.AGhostIsWhatIBe;
    }
    return *this;
}