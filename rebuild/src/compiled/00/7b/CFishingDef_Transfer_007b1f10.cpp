// CFishingDef::Transfer(CPersistContext& context) -- persist the fishing definition: the FishTypes vector, the fishing-spot geometry, fishing speeds, weather weight multipliers, wait/hook timings and hook-press counts
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B1F10, length 1016 bytes, virtual, `ret 4`.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (x19, the empty key "" @ 0x0122D70E), the vector<long> writer @ 0x00466A75 and
// reader @ 0x00466AA7 (`__fastcall` (stream, vector*), labels propagated), CDataOutputStream::WriteFloat @ 0x00993EE0 (manifest
// "WriteCBYTE" is propagated), CPersistTraits<float>::TransferBinaryIn @ 0x0040EFB0, CDataOutputStream::WriteLong @ 0x00993EB0,
// CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60.
// Shape: the landed CCreatureGeneratorDef::Transfer 0x0074C5F0 template -- nineteen inline CPersistContext transfers; the vector
// transfer is the two-way form (mode 2 loads, anything else saves), the scalars the `sub eax,2 ; je ; dec eax ; jne` switch with the
// save arm inline and the load through the traits temporary in the dead argument slot (`lea ecx,[esp+18h]`).
// Layout: Ego_r.pdb CFishingDef (0x80) with a 16-byte FishTypes vector; retail STLport vectors are 12 bytes, so every member after
// it sits 4 below the PDB: FishTypes +0x28, FishingSpotProximity +0x34 ... StrainIncreasePerPress +0x78 (no engine header).
class CMemoryDataInputStream;

class CDataOutputStream { public:
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};

struct CLongVector {                                    // std::vector<long> (retail 12 bytes)
    long* _First;
    long* _Last;
    long* _End;
};
void __fastcall GFSerialiseVectorBinaryOutLong(CDataOutputStream* stream, CLongVector* value);     // 0x00466A75
void __fastcall GFSerialiseVectorBinaryInLong(CMemoryDataInputStream* stream, CLongVector* value); // 0x00466AA7

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);   // 0x0040FE60
    static void TransferBinaryOut(CDataOutputStream* stream, const long& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);  // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    protected: void CheckCRC(const char* name); public:                    // 0x00404500
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
};

class CFishingDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _pad04[0x28 - 4];
    CLongVector FishTypes;                              // +0x28
    float FishingSpotProximity;                         // +0x34
    float FishingSpotMinDepth;                          // +0x38
    float FishingSpotMinHeightDiff;                     // +0x3c
    float FishingSpotMaxHeightDiff;                     // +0x40
    long  PointsPerFishingLevel;                        // +0x44
    float SlowFishingSpeed;                             // +0x48
    float NormalFishingSpeed;                           // +0x4c
    float FastFishingSpeed;                             // +0x50
    float FastestFishingSpeed;                          // +0x54
    float FishWeightRainMult;                           // +0x58
    float FishWeightSnowMult;                           // +0x5c
    float FishWeightMistMult;                           // +0x60
    float BaseWaitPeriodSeconds;                        // +0x64
    float MaxRandomWaitPeriodSeconds;                   // +0x68
    float HookWindowSeconds;                            // +0x6c
    long  BaseHookPresses;                              // +0x70
    long  MaxRandomHookPresses;                         // +0x74
    float StrainIncreasePerPress;                       // +0x78
};

void CFishingDef::Transfer(CPersistContext& context)
{
    context.Transfer("", FishTypes);
    context.Transfer("", FishingSpotProximity);
    context.Transfer("", FishingSpotMinDepth);
    context.Transfer("", FishingSpotMinHeightDiff);
    context.Transfer("", FishingSpotMaxHeightDiff);
    context.Transfer("", PointsPerFishingLevel);
    context.Transfer("", SlowFishingSpeed);
    context.Transfer("", NormalFishingSpeed);
    context.Transfer("", FastFishingSpeed);
    context.Transfer("", FastestFishingSpeed);
    context.Transfer("", FishWeightRainMult);
    context.Transfer("", FishWeightSnowMult);
    context.Transfer("", FishWeightMistMult);
    context.Transfer("", BaseWaitPeriodSeconds);
    context.Transfer("", MaxRandomWaitPeriodSeconds);
    context.Transfer("", HookWindowSeconds);
    context.Transfer("", BaseHookPresses);
    context.Transfer("", MaxRandomHookPresses);
    context.Transfer("", StrainIncreasePerPress);
}