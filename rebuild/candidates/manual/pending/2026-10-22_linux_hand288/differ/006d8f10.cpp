// CHeroExperienceDef::Transfer(CPersistContext& context) -- persist the hero experience definition: level thresholds, level titles, fallback multipliers, sound-intensity and combo-multiplier maps, the two combo experience multipliers, the stat-increase-per-hit-type map, the four combo sound maps and the attack-history combos
// Grade: DIFFER(693v692) -- only the ebp save differs: retail pushes ebp right before the first StatIncreasePerHitType transfer and pops it before the last branch (shrink-wrapped); ours saves it in the prologue
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D8F10, length 692 bytes (virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC 0x00404500 (x14, the empty key "" 0x0122D70E);
//   vector<long> writer/reader 0x00466A75 / 0x00466AA7 (`__fastcall` (stream, vector*); the manifest's
//   "GFSerialiseVectorBinaryOut<float>" / "...In<CRGBColour>" labels are propagated);
//   map<long,CDefString> writer/reader 0x006DAE40 / 0x006DBA50 (`__fastcall`; the reader's manifest label
//   "CPersistTraits<list<COpinionTransientOffset>>::TransferBinaryIn" is propagated);
//   map<long,CComboMultiplierData> writer/reader 0x006DC350 / 0x006DCFA0;
//   map<EDamageAttribute,CHeroStatIncreaseData> writer/reader 0x006DC450 / 0x006DD1C0 (catalog "WriteToStream"/"ReadFromStream");
//   vector<CAttackHistoryCombo> writer/reader 0x006DC550 / 0x006DD580 (manifest "<CMusicEntry>"/"<CBrainUpdateZone>" propagated);
//   CDataOutputStream::WriteFloat 0x00993EE0 (manifest "WriteCBYTE"), CPersistTraits<float>::TransferBinaryIn 0x0040EFB0.
// Shape: fourteen inline CPersistContext transfers (the landed CCreatureGeneratorDef::Transfer 0x0074C5F0 template): every
//   member address is formed before its CRC call; vectors use the two-way form (mode 2 loads, anything else saves), maps and
//   floats the `sub eax,2 ; je ; dec eax ; jne` switch; the float load uses the empty traits temporary in the dead argument
//   slot. Retail transfers StatIncreasePerHitType twice -- once after the two floats and again after the four combo sound
//   maps (the second time it reuses the `ebp` address formed for the first).
// Layout: Ego_r.pdb CHeroExperienceDef (0xe0) re-based on the retail bytes: every STLport vector and map is 12 bytes
//   (16 in the PDB): ExperienceLevelThresholds +0x28 .. AttackHistoryCombos +0xa8. No generated engine header for the class.
struct CMemoryDataInputStream;
struct CDataOutputStream {
    void WriteFloat(float value);                       // 0x00993EE0
};

struct CLongVector { long* _M_start; long* _M_finish; long* _M_end_of_storage; };
struct CAttackHistoryComboVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct CDefStringMap { void* _M_header[3]; };                     // map<long, CDefString>
struct CComboMultiplierMap { void* _M_header[3]; };               // map<long, CComboMultiplierData>
struct CStatIncreaseMap { void* _M_header[3]; };                  // map<EDamageAttribute, CHeroStatIncreaseData>

void __fastcall GFSerialiseVectorBinaryOutLong(CDataOutputStream* stream, CLongVector* value);       // 0x00466A75
void __fastcall GFSerialiseVectorBinaryInLong(CMemoryDataInputStream* stream, CLongVector* value);   // 0x00466AA7
void __fastcall GFSerialiseVectorBinaryOutCombo(CDataOutputStream* stream, CAttackHistoryComboVector* value);     // 0x006DC550
void __fastcall GFSerialiseVectorBinaryInCombo(CMemoryDataInputStream* stream, CAttackHistoryComboVector* value); // 0x006DD580
void __fastcall WriteDefStringMap(CDataOutputStream* stream, CDefStringMap* value);              // 0x006DAE40
void __fastcall ReadDefStringMap(CMemoryDataInputStream* stream, CDefStringMap* value);          // 0x006DBA50
void __fastcall WriteComboMultiplierMap(CDataOutputStream* stream, CComboMultiplierMap* value);  // 0x006DC350
void __fastcall ReadComboMultiplierMap(CMemoryDataInputStream* stream, CComboMultiplierMap* value); // 0x006DCFA0
void __fastcall WriteStatIncreaseMap(CDataOutputStream* stream, CStatIncreaseMap* value);        // 0x006DC450
void __fastcall ReadStatIncreaseMap(CMemoryDataInputStream* stream, CStatIncreaseMap* value);    // 0x006DD1C0

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);   // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};
template<> struct CPersistTraits<CDefStringMap> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CDefStringMap* value) { ReadDefStringMap(stream, value); }
    static void TransferBinaryOut(CDataOutputStream* stream, CDefStringMap& value) { WriteDefStringMap(stream, &value); }
};
template<> struct CPersistTraits<CComboMultiplierMap> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CComboMultiplierMap* value) { ReadComboMultiplierMap(stream, value); }
    static void TransferBinaryOut(CDataOutputStream* stream, CComboMultiplierMap& value) { WriteComboMultiplierMap(stream, &value); }
};
template<> struct CPersistTraits<CStatIncreaseMap> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CStatIncreaseMap* value) { ReadStatIncreaseMap(stream, value); }
    static void TransferBinaryOut(CDataOutputStream* stream, CStatIncreaseMap& value) { WriteStatIncreaseMap(stream, &value); }
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
    void Transfer(const char* name, CAttackHistoryComboVector& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseVectorBinaryInCombo(m_pLoadStream, &value);
            break;
        default:
            GFSerialiseVectorBinaryOutCombo(m_pSaveStream, &value);
            break;
        }
    }
};

struct CHeroExperienceDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char       _pad04[0x28 - 4];
    CLongVector         ExperienceLevelThresholds;                        // +0x28
    CLongVector         HeroLevelTitles;                                  // +0x34
    CLongVector         FallbackMultipliers;                              // +0x40
    CDefStringMap       SoundIntensityThresholds;                         // +0x4c
    CComboMultiplierMap ComboMultiplierMap;                               // +0x58
    float               ExperienceMultiplierParamPerUnitComboMultiplier;  // +0x64
    float               ExperienceMultiplierRegionChangeMultiplier;       // +0x68
    CDefStringMap       ComboMultiplierIncrease;                          // +0x6c
    CDefStringMap       ComboMultiplierLock;                              // +0x78
    CDefStringMap       ComboMultiplierLost;                              // +0x84
    CDefStringMap       ComboMultiplierRunningHitsIncrease;               // +0x90
    CStatIncreaseMap    StatIncreasePerHitType;                           // +0x9c
    CAttackHistoryComboVector AttackHistoryCombos;                        // +0xa8
};

void CHeroExperienceDef::Transfer(CPersistContext& context)
{
    context.Transfer("", ExperienceLevelThresholds);
    context.Transfer("", HeroLevelTitles);
    context.Transfer("", FallbackMultipliers);
    context.Transfer("", SoundIntensityThresholds);
    context.Transfer("", ComboMultiplierMap);
    context.Transfer("", ExperienceMultiplierParamPerUnitComboMultiplier);
    context.Transfer("", ExperienceMultiplierRegionChangeMultiplier);
    context.Transfer("", StatIncreasePerHitType);
    context.Transfer("", ComboMultiplierIncrease);
    context.Transfer("", ComboMultiplierLock);
    context.Transfer("", ComboMultiplierLost);
    context.Transfer("", ComboMultiplierRunningHitsIncrease);
    context.Transfer("", StatIncreasePerHitType);
    context.Transfer("", AttackHistoryCombos);
}
