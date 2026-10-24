#include <cstdio>
#include <cstring>
struct CMemoryDataInputStream;

struct CDataOutputStream {
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);    // 0x0040FE60
    static void TransferBinaryOut(CDataOutputStream* stream, const long& value) { stream->WriteLong(value); }
};
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);   // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};

class CShoveHaPennyRegionDef;
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };   // retail 12-byte STLport vector

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template <class T> void TransferVectorOfSubComponents(vector<T>& value);   // <CShoveHaPennyRegionDef> 0x007D2740
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
};

class CTavernGameDef {
public:
    virtual void Transfer(CPersistContext& context);    // 0x004ED472
    unsigned char _body04[0x1A0];                       // +0x04..+0x1A3 (retail CTavernGameDef is 0x1A4)
};

class CTavernGameShoveHaPennyDef : public CTavernGameDef {
public:
    vector<CShoveHaPennyRegionDef> Regions;             // +0x1A4
    long  BoardDefIndex;                                // +0x1B0
    long  ArrowDefIndex;                                // +0x1B4
    float PositionCoinSpeedXbox;                        // +0x1B8
    float PositionCoinSpeedPC;                          // +0x1BC
    float PositionCoinMinimum;                          // +0x1C0
    float PositionCoinMaximum;                          // +0x1C4
    float PositionCoinDepth;                            // +0x1C8
    float ImpulseScale;                                 // +0x1CC
    float TargetDistanceScale;                          // +0x1D0
    float JoystickScaleXboxX;                           // +0x1D4
    float JoystickScaleXboxY;                           // +0x1D8
    float JoystickScalePCX;                             // +0x1DC
    float JoystickScalePCY;                             // +0x1E0
    float JoystickTranslateY;                           // +0x1E4
    float JoystickMagBelow;                             // +0x1E8
    float AverageJoystickMagAbove;                      // +0x1EC
    float AverageDrift;                                 // +0x1F0
    float MaxPower;                                     // +0x1F4
    float RestSpeed;                                    // +0x1F8
    long  Rounds;                                       // +0x1FC
    virtual void Transfer(CPersistContext& context);
};

void CTavernGameShoveHaPennyDef::Transfer(CPersistContext& context)
{
    CTavernGameDef::Transfer(context);
    context.TransferVectorOfSubComponents(Regions);
    context.Transfer("", BoardDefIndex);
    context.Transfer("", ArrowDefIndex);
    context.Transfer("", ImpulseScale);
    context.Transfer("", TargetDistanceScale);
    context.Transfer("", JoystickScaleXboxX);
    context.Transfer("", JoystickScaleXboxY);
    context.Transfer("", JoystickScalePCX);
    context.Transfer("", JoystickScalePCY);
    context.Transfer("", JoystickTranslateY);
    context.Transfer("", JoystickMagBelow);
    context.Transfer("", AverageJoystickMagAbove);
    context.Transfer("", MaxPower);
    context.Transfer("", AverageDrift);
    context.Transfer("", PositionCoinSpeedXbox);
    context.Transfer("", PositionCoinSpeedPC);
    context.Transfer("", PositionCoinMinimum);
    context.Transfer("", PositionCoinMaximum);
    context.Transfer("", PositionCoinDepth);
    context.Transfer("", RestSpeed);
    context.Transfer("", Rounds);
}

static int g_crc = 0, g_base = 0, g_regions = 0, g_writes = 0, g_loads = 0;
static float g_written[64];
static float g_nextLoad = 100.0f;
void CPersistContext::CheckCRC(const char* name) { if (name[0] == 0) ++g_crc; }
template <class T> void CPersistContext::TransferVectorOfSubComponents(vector<T>& value) { (void)value; ++g_regions; }
void CTavernGameDef::Transfer(CPersistContext& context) { (void)context; ++g_base; }
void CDataOutputStream::WriteLong(long value) { g_written[g_writes++] = (float)value; }
void CDataOutputStream::WriteFloat(float value) { g_written[g_writes++] = value; }
void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream* stream, long* value) { (void)stream; *value = (long)g_nextLoad; g_nextLoad += 1.0f; ++g_loads; }
void CPersistTraits<float>::TransferBinaryIn(CMemoryDataInputStream* stream, float* value) { (void)stream; *value = g_nextLoad; g_nextLoad += 1.0f; ++g_loads; }

int main()
{
    static CTavernGameShoveHaPennyDef def;
    static CPersistContext context;
    static CDataOutputStream out;
    bool ok = true;
    // save: every scalar written in persist order
    float* order[] = { 0, 0, &def.ImpulseScale, &def.TargetDistanceScale, &def.JoystickScaleXboxX, &def.JoystickScaleXboxY,
        &def.JoystickScalePCX, &def.JoystickScalePCY, &def.JoystickTranslateY, &def.JoystickMagBelow,
        &def.AverageJoystickMagAbove, &def.MaxPower, &def.AverageDrift, &def.PositionCoinSpeedXbox,
        &def.PositionCoinSpeedPC, &def.PositionCoinMinimum, &def.PositionCoinMaximum, &def.PositionCoinDepth, &def.RestSpeed, 0 };
    def.BoardDefIndex = 7; def.ArrowDefIndex = 8; def.Rounds = 9;
    for (int i = 2; i < 19; ++i) *order[i] = 0.5f * (float)i;
    context.m_mode = 3; context.m_pSaveStream = &out;
    def.Transfer(context);
    if (g_base != 1 || g_regions != 1 || g_crc != 20 || g_writes != 20 || g_loads != 0) ok = false;
    if (g_written[0] != 7.0f || g_written[1] != 8.0f || g_written[19] != 9.0f) ok = false;
    for (int i = 2; i < 19; ++i) if (g_written[i] != 0.5f * (float)i) ok = false;
    // load: each member receives the next value from the stream in persist order
    context.m_mode = 2; g_writes = 0;
    def.Transfer(context);
    if (g_loads != 20 || g_writes != 0 || g_crc != 40) ok = false;
    if (def.BoardDefIndex != 100 || def.ArrowDefIndex != 101 || def.Rounds != 119) ok = false;
    for (int i = 2; i < 19; ++i) if (*order[i] != 100.0f + (float)i) ok = false;
    // any other mode only checks the keys
    context.m_mode = 0;
    def.Transfer(context);
    if (g_loads != 20 || g_writes != 0 || g_crc != 60 || g_base != 3) ok = false;
    if (!ok) { std::printf("007d0a40_TEST FAIL\n"); return 1; }
    std::printf("007d0a40_TEST PASS\n");
    return 0;
}