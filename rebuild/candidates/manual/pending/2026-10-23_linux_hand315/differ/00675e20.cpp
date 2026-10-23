// Grade: DIFFER(1912v1912) -- register rotation only: retail uses eax for every scratch load in the four counted-pointer transfers (both arms, the 0x00676740 argument, the stats vtable), so its long transfers start the eax/ecx/edx rotation one step later; ours rotates from the first block (QFE 4035 gives the same)
// CCreatureDef::Transfer(CPersistContext&) -- persist a creature def: the wound morphs, random appearance morph, expression set and stats def behind their counted pointers, the initial action / appearance-modifier vectors, phoneme animation map, dialogue voices, the inventory and screen inventory ids, creature group, battle-cry sound, opinion source, respawn waits, the six flags, the radius weight and spread, and AGhostIsWhatIBe
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00675E20, length 1910 bytes, virtual, `ret 4`.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key "" @ 0x0122D70E, once per member), CWoundMorphs::TransferBinaryOut
// @ 0x00757500 / TransferBinaryIn @ 0x00757990, the out-of-line CRandomAppearanceMorph transfer @ 0x00676740, CExpressionSet
// TransferBinaryOut @ 0x0086F590 / In @ 0x0086F680, the stats def's virtual Transfer (slot 18, +0x48), the vector<long> writer
// @ 0x00466A75 / reader @ 0x00466AA7, the map<CCharString,long> writer @ 0x004314C8 / reader @ 0x0043151D, the vector<CDefString>
// writer @ 0x00465820 / reader @ 0x00465852 (all `__fastcall` (stream, container*)), CDataOutputStream::WriteLong @ 0x00993EB0 /
// WriteBool @ 0x00993E30, CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60, CPersistTraits<unsigned long>::TransferBinaryIn
// @ 0x0040F8A0 (manifest "<float>"), CDefString::TransferBinaryIn @ 0x0044FC00, CPersistTraits<bool>::TransferBinaryIn @ 0x00403EB0,
// the CHeroAbilityStatWeight writer @ 0x00466848 / reader @ 0x00466889, the out-of-line CFloatRange transfer @ 0x00458F2B.
// Shape: the landed CCreatureGeneratorDef::Transfer (0x0074C5F0) inline CPersistContext::Transfer template per member, in the
// PDB declaration order except that MagicScreenInventory precedes StatsScreenInventory and UseActualHitPosForHitEffects leads the
// flags; containers with a two-way writer use the `default:` save arm.
// Layout: Ego_r.pdb CCreatureDef (252 bytes) shifted by the retail 12-byte vectors / maps (PDB 16): PWoundMorphs +0x28,
// PRandomAppearanceMorph +0x30, PExpressions +0x38, PStats +0x40, InitialActions +0x48, InitialAppearanceModifiers +0x54,
// PhonemeAnim +0x60, DialogueVoices +0x6c, Inventory .. InGameMenuScreen +0x78..+0xa8, CreatureGroup +0xac, BattleCrySound +0xb0,
// OpinionSourceDef +0xb4, RespawnWait* +0xb8..+0xc4, the flags +0xc8..+0xcd, RadiusWeight +0xd0, RadiusSpread +0xdc,
// AGhostIsWhatIBe +0xe4 (no engine header for this class).
struct CMemoryDataInputStream;
struct CPersistContext;

struct CDataOutputStream {
    void WriteLong(long value);                         // 0x00993EB0
    void WriteBool(bool value);                         // 0x00993E30
};

struct CDefString {
    long TablePos;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

struct CWoundMorphs {
    void TransferBinaryOut(CDataOutputStream* stream) const;   // 0x00757500
    void TransferBinaryIn(CMemoryDataInputStream* stream);     // 0x00757990
};
struct CExpressionSet {
    void TransferBinaryOut(CDataOutputStream* stream) const;   // 0x0086F590
    void TransferBinaryIn(CMemoryDataInputStream* stream);     // 0x0086F680
};
struct CRandomAppearanceMorph;
struct CCreatureStatsDef {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Transfer(CPersistContext& context);    // slot 18 (+0x48)
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T& operator*() const { return *Data; }
};

struct CLongVector { long* _First; long* _Last; long* _End; };                       // vector<long> (retail 12 bytes)
struct CDefStringVector { CDefString* _First; CDefString* _Last; CDefString* _End; };  // vector<CDefString>
struct CStringLongMap { void* _Header; unsigned long _Size; long _Pad; };            // map<CCharString,long> (retail 12 bytes)
struct CHeroAbilityStatWeight { void* _Header; unsigned long _Size; long _Pad; };    // retail 12 bytes
struct CFloatRange { float Min; float Max; };

void __fastcall GFSerialiseVectorBinaryOutLong(CDataOutputStream* stream, CLongVector* value);           // 0x00466A75
void __fastcall GFSerialiseVectorBinaryInLong(CMemoryDataInputStream* stream, CLongVector* value);       // 0x00466AA7
void __fastcall GFSerialiseMapBinaryOutPhonemes(CDataOutputStream* stream, CStringLongMap* value);       // 0x004314C8
void __fastcall GFSerialiseMapBinaryInPhonemes(CMemoryDataInputStream* stream, CStringLongMap* value);   // 0x0043151D
void __fastcall GFSerialiseVectorBinaryOutDefString(CDataOutputStream* stream, CDefStringVector* value);     // 0x00465820
void __fastcall GFSerialiseVectorBinaryInDefString(CMemoryDataInputStream* stream, CDefStringVector* value); // 0x00465852
void __fastcall GFSerialiseStatWeightBinaryOut(CDataOutputStream* stream, CHeroAbilityStatWeight* value);     // 0x00466848
void __fastcall GFSerialiseStatWeightBinaryIn(CMemoryDataInputStream* stream, CHeroAbilityStatWeight* value); // 0x00466889

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);   // 0x0040FE60
    static void TransferBinaryOut(CDataOutputStream* stream, const long& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<unsigned long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, unsigned long* value);   // 0x0040F8A0
    static void TransferBinaryOut(CDataOutputStream* stream, const unsigned long& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};
template<> struct CPersistTraits<CDefString> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CDefString* value) { value->TransferBinaryIn(stream); }
    static void TransferBinaryOut(CDataOutputStream* stream, const CDefString& value) { stream->WriteLong(value.TablePos); }
};
struct CPersistContext {
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
    void Transfer(const char* name, CWoundMorphs& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            value.TransferBinaryIn(m_pLoadStream);
            break;
        case 3:
            value.TransferBinaryOut(m_pSaveStream);
            break;
        }
    }
    void Transfer(const char* name, CExpressionSet& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            value.TransferBinaryIn(m_pLoadStream);
            break;
        case 3:
            value.TransferBinaryOut(m_pSaveStream);
            break;
        }
    }
    void Transfer(const char* name, CCreatureStatsDef& value)
    {
        CheckCRC(name);
        value.Transfer(*this);
    }
    void Transfer(const char* name, CLongVector& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseVectorBinaryInLong(m_pLoadStream, &value);
            break;
        default:
            GFSerialiseVectorBinaryOutLong(m_pSaveStream, &value);
            break;
        }
    }
    void Transfer(const char* name, CStringLongMap& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseMapBinaryInPhonemes(m_pLoadStream, &value);
            break;
        case 3:
            GFSerialiseMapBinaryOutPhonemes(m_pSaveStream, &value);
            break;
        }
    }
    void Transfer(const char* name, CDefStringVector& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseVectorBinaryInDefString(m_pLoadStream, &value);
            break;
        default:
            GFSerialiseVectorBinaryOutDefString(m_pSaveStream, &value);
            break;
        }
    }
    void Transfer(const char* name, CHeroAbilityStatWeight& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseStatWeightBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            GFSerialiseStatWeightBinaryOut(m_pSaveStream, &value);
            break;
        }
    }
    void TransferRandomAppearanceMorph(CRandomAppearanceMorph& value);   // 0x00676740 (CRC "" inside)
    void TransferFloatRange(CFloatRange& value);                         // 0x00458F2B (CRC "" inside)
};

struct CCreatureDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _pad04[0x28 - 4];
    CCountedPointer<CWoundMorphs>           PWoundMorphs;            // +0x28
    CCountedPointer<CRandomAppearanceMorph> PRandomAppearanceMorph;  // +0x30
    CCountedPointer<CExpressionSet>         PExpressions;            // +0x38
    CCountedPointer<CCreatureStatsDef>      PStats;                  // +0x40
    CLongVector       InitialActions;                                // +0x48
    CLongVector       InitialAppearanceModifiers;                    // +0x54
    CStringLongMap    PhonemeAnim;                                   // +0x60
    CDefStringVector  DialogueVoices;                                // +0x6c
    long Inventory;                                                  // +0x78
    long ClothingInventory;                                          // +0x7c
    long WeaponsInventory;                                           // +0x80
    long HeroAbilitiesScreenInventory;                               // +0x84
    long QuestCardScreenInventory;                                   // +0x88
    long MapScreenInventory;                                         // +0x8c
    long StatsScreenInventory;                                       // +0x90
    long MagicScreenInventory;                                       // +0x94
    long ExperienceScreenInventory;                                  // +0x98
    long TradeScreenInventory;                                       // +0x9c
    long AugmentScreenInventory;                                     // +0xa0
    long QuestsScreenInventory;                                      // +0xa4
    long InGameMenuScreen;                                           // +0xa8
    unsigned long CreatureGroup;                                     // +0xac
    CDefString BattleCrySound;                                       // +0xb0
    long OpinionSourceDef;                                           // +0xb4
    long RespawnWaitInDaysMin;                                       // +0xb8
    long RespawnWaitInDaysMax;                                       // +0xbc
    long RespawnWaitInFramesMin;                                     // +0xc0
    long RespawnWaitInFramesMax;                                     // +0xc4
    bool Short;                                                      // +0xc8
    bool FlashOnHit;                                                 // +0xc9
    bool PickPocketable;                                             // +0xca
    bool ChildThatCanRegionFollow;                                   // +0xcb
    bool AllowedInProtectedTowns;                                    // +0xcc
    bool UseActualHitPosForHitEffects;                               // +0xcd
    CHeroAbilityStatWeight RadiusWeight;                             // +0xd0
    CFloatRange RadiusSpread;                                        // +0xdc
    bool AGhostIsWhatIBe;                                            // +0xe4
};

void CCreatureDef::Transfer(CPersistContext& context)
{
    context.Transfer("", *PWoundMorphs);
    context.TransferRandomAppearanceMorph(*PRandomAppearanceMorph);
    context.Transfer("", *PExpressions);
    context.Transfer("", *PStats);
    context.Transfer("", InitialActions);
    context.Transfer("", InitialAppearanceModifiers);
    context.Transfer("", PhonemeAnim);
    context.Transfer("", DialogueVoices);
    context.Transfer("", Inventory);
    context.Transfer("", ClothingInventory);
    context.Transfer("", WeaponsInventory);
    context.Transfer("", HeroAbilitiesScreenInventory);
    context.Transfer("", QuestCardScreenInventory);
    context.Transfer("", MapScreenInventory);
    context.Transfer("", MagicScreenInventory);
    context.Transfer("", StatsScreenInventory);
    context.Transfer("", ExperienceScreenInventory);
    context.Transfer("", TradeScreenInventory);
    context.Transfer("", AugmentScreenInventory);
    context.Transfer("", QuestsScreenInventory);
    context.Transfer("", InGameMenuScreen);
    context.Transfer("", CreatureGroup);
    context.Transfer("", BattleCrySound);
    context.Transfer("", OpinionSourceDef);
    context.Transfer("", RespawnWaitInDaysMin);
    context.Transfer("", RespawnWaitInDaysMax);
    context.Transfer("", RespawnWaitInFramesMin);
    context.Transfer("", RespawnWaitInFramesMax);
    context.Transfer("", UseActualHitPosForHitEffects);
    context.Transfer("", Short);
    context.Transfer("", FlashOnHit);
    context.Transfer("", PickPocketable);
    context.Transfer("", ChildThatCanRegionFollow);
    context.Transfer("", AllowedInProtectedTowns);
    context.Transfer("", RadiusWeight);
    context.TransferFloatRange(RadiusSpread);
    context.Transfer("", AGhostIsWhatIBe);
}
