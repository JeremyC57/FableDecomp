// Behaviour test for 0x00675C00 (copy of the model + stand-in callee bodies).
#include <cstdio>
#include <cstring>
// CCreatureDef::operator=(const CCreatureDef&) -- self-checked def assignment: deep-copies the four counted-pointer payloads (wound morphs, random appearance morph, expression set, creature stats), then the action/appearance vectors, phoneme map, dialogue voices, the inventory/screen defs, respawn timings, flags, radius weight/spread; returns *this
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
struct CWoundMorphs { CWoundMorphMap Morphs; };
struct CExpressionSet { CExpressionMap Expressions; };
struct CRandomAppearanceMorph {
    unsigned char _body[0x40];
    CRandomAppearanceMorph& operator=(const CRandomAppearanceMorph& other);   // 0x00675B60
};

struct CDefClassBase {
    unsigned char _body[0x28];              // vptr, resource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CCreatureStatsDef : CDefClassBase {
    long MoralityForKill;                   // +0x28
    long MoralityForPunch;                  // +0x2c
    long MoralityForWeaponHit;              // +0x30
    long ExperienceWorth;                   // +0x34
    long RenownWorth;                       // +0x38
    float Speed;                            // +0x3c
    float Dexterity;                        // +0x40
    float StrengthDamageMultiplier;         // +0x44
};

template <class T> struct CCountedPointer {  // object pointer + shared count (8 bytes)
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
struct CDefString { long Index; };
struct CHeroAbilityStatWeight {             // map<EHeroTrainableStatType,float> StatWeights (12 bytes)
    void* m_head; unsigned long m_size; unsigned long m_compare;
    CHeroAbilityStatWeight& operator=(const CHeroAbilityStatWeight& other);   // 0x0046230E
};
struct CFloatRange { float Min; float Max; };

struct CCreatureDef : CDefClassBase {                                // vtable 0x0125B43C
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

CWoundMorphEntryVector& CWoundMorphEntryVector::operator=(const CWoundMorphEntryVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }
CExpressionEntryVector& CExpressionEntryVector::operator=(const CExpressionEntryVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }
CRandomAppearanceMorph& CRandomAppearanceMorph::operator=(const CRandomAppearanceMorph& other) { std::memcpy(_body, other._body, sizeof(_body)); return *this; }
CDefClassBase& CDefClassBase::operator=(const CDefClassBase& other) { std::memcpy(_body, other._body, sizeof(_body)); return *this; }
CLongVector& CLongVector::operator=(const CLongVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }
CPhonemeAnimMap& CPhonemeAnimMap::operator=(const CPhonemeAnimMap& other) { m_head = other.m_head; m_size = other.m_size; return *this; }
CDefStringVector& CDefStringVector::operator=(const CDefStringVector& other) { m_first = other.m_first; m_last = other.m_last; m_end = other.m_end; return *this; }
CHeroAbilityStatWeight& CHeroAbilityStatWeight::operator=(const CHeroAbilityStatWeight& other) { m_head = other.m_head; m_size = other.m_size; return *this; }

int main()
{
    static CWoundMorphs wa, wb; static CRandomAppearanceMorph ra, rb; static CExpressionSet ea, eb; static CCreatureStatsDef sa, sb;
    static CCreatureDef a, b;
    a.PWoundMorphs.m_pObject = &wa; b.PWoundMorphs.m_pObject = &wb;
    a.PRandomAppearanceMorph.m_pObject = &ra; b.PRandomAppearanceMorph.m_pObject = &rb;
    a.PExpressions.m_pObject = &ea; b.PExpressions.m_pObject = &eb;
    a.PStats.m_pObject = &sa; b.PStats.m_pObject = &sb;
    wb.Morphs.m_sorted = true; wb.Morphs.m_entries.m_last = &wb; rb._body[7] = 3; eb.Expressions.m_sorted = true;
    sb.RenownWorth = 50; sb.Dexterity = 0.75f; sb._body[2] = 8;
    b.InitialActions.m_first = 0; b.PhonemeAnim.m_size = 4; b.StatsScreenInventory = 90; b.MagicScreenInventory = 94;
    b.UseActualHitPosForHitEffects = true; b.Short = true; b.RadiusWeight.m_size = 2; b.RadiusSpread.Max = 0.5f;
    b.AGhostIsWhatIBe = true; b.BattleCrySound.Index = 6;
    CCreatureDef* r = &(a = b);
    if (r != &a || a.PWoundMorphs.m_pObject != &wa || !wa.Morphs.m_sorted || wa.Morphs.m_entries.m_last != &wb
        || ra._body[7] != 3 || !ea.Expressions.m_sorted || sa.RenownWorth != 50 || sa.Dexterity != 0.75f || sa._body[2] != 8
        || a.PhonemeAnim.m_size != 4 || a.StatsScreenInventory != 90 || a.MagicScreenInventory != 94
        || !a.UseActualHitPosForHitEffects || !a.Short || a.RadiusWeight.m_size != 2 || a.RadiusSpread.Max != 0.5f
        || !a.AGhostIsWhatIBe || a.BattleCrySound.Index != 6) { std::printf("00675c00_TEST FAIL\n"); return 1; }
    a.Inventory = 1;
    CCreatureDef* r2 = &(a = a);
    if (r2 != &a || a.Inventory != 1) { std::printf("00675c00_TEST FAIL\n"); return 1; }
    std::printf("00675c00_TEST PASS\n");
    return 0;
}
