// NParticleEngine::CPSCLight::DatabaseSerialise(CPersistContext& context, bool) -- persist the light settings by name: enable/life/respawn flags and times, render radii, attenuation and fade integers, colour-fade flags and colours, timeline, then the user points array size and each point
// Grade: DIFFER(1310v1310) -- one register choice: the LightWorldRadiusFadeEnable (bit 0) write-back loads the bool into al before `xor cl,al`; retail folds it as `xor cl,[esp+30h]` (the shifted loop-alignment lea follows from that one byte)
// Leaf: DatabaseSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1A870, 1310 bytes (`ret 8`).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer(name, unsigned long&, ...) 0x004106F0,
//   Transfer(name, CCharString&, ...) 0x004109A0, Transfer(name, float&, ...) 0x00410620, Transfer(name, long&, ...)
//   0x00410BE0, Transfer(name, CRGBColour&, ...) 0x006420F0, Transfer(name, C3DVector&, ...) 0x004859AD; CCharString()
//   0x0099E4B0, CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0; CRC Calc 0x004014A0;
//   CArray<C3DVector>::resize 0x0075C040.
// Data: names "LightEnabled" .. "LightUserPointsArray" 0x012A08E8..0x012A06A8, empty string 0x0129AAF4.
// Layout: PDB (Ego_r) CPSCLight (0x70 bytes), retail offsets equal the PDB: flag bits +0x48 (WorldRadiusFadeEnable,
//   UseLifeSecs, Enabled, Respawns, ColourFadeEnable, UseStart/Mid/EndColour) and +0x49 (UseFadeColour,
//   UseTimelineSecs, StartedTimer); LightUserPointsArray is a 12-byte STLport vector at +0x60.
// Shape: the landed CPSCAttractor::DatabaseSerialise (0x00B18110) idiom: each bit flag through its own bool block,
//   the position param as a CRC when loading (inline by-value empty default + if/else CRC helper), CRGBColour
//   defaults as (0,0,0,0xff) temporaries whose ctor assigns R, G, B, A, and the guarded point loop.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRGBColour {                                    // union { struct { B, G, R, A }; unsigned long Colour; } (4-aligned)
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CCharStringData {
    const char* Data;                                   // +0
    long Length;                                        // +4
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
static inline CCharString EmptyCCharString()
{
    CCharString empty;
    return empty;
}
unsigned long __fastcall CRCCalc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0
inline unsigned long GetCRC(const CCharString& s)
{
    const char* data;
    unsigned long length;
    if (s.PStringData == 0) {
        length = 0;
        data = "";
    } else {
        length = s.PStringData->Length;
        data = s.PStringData->Data;
    }
    return CRCCalc(0, data, length);
}

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);                       // 0x004045C0
    void Transfer(const char* name, unsigned long& value, const unsigned long& defaultValue);     // 0x004106F0
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);         // 0x004109A0
    void Transfer(const char* name, float& value, const float& defaultValue);                     // 0x00410620
    void Transfer(const char* name, long& value, const long& defaultValue);                       // 0x00410BE0
    void Transfer(const char* name, CRGBColour& value, const CRGBColour& defaultValue);           // 0x006420F0
    void Transfer(const char* name, C3DVector& value, const C3DVector& defaultValue);             // 0x004859AD
};
struct CVector3D {                                      // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    C3DVector& operator[](unsigned long n) { return _M_start[n]; }
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
};
struct CEnginePrimitiveHandle {
    void* m_pPrimitive;
    void* m_pOwner;
};
namespace NParticleEngine {
struct CPSCLight {
    unsigned char _base[0xc];                           // CPSCBase
    long LightLifeTimer;                                // +0x0c
    long LightRespawnDelayTimer;                        // +0x10
    bool LightIsAlive;                                  // +0x14
    unsigned long LightPositionParam;                   // +0x18
    float LightLifeSecs;                                // +0x1c
    float LightRespawnDelaySecs;                        // +0x20
    float LightStartTime;                               // +0x24
    float LightTimelineSecs;                            // +0x28
    float LightStartRenderWorldRadius;                  // +0x2c
    float LightEndRenderWorldRadius;                    // +0x30
    long LightAttenuationFactor;                        // +0x34
    long LightFadeInEndInteger;                         // +0x38
    long LightFadeOutBeginInteger;                      // +0x3c
    float LightWorldRadiusFadeMinimum;                  // +0x40
    CRGBColour LightColourFadeMinimum;                  // +0x44
    bool LightWorldRadiusFadeEnable : 1;                // +0x48 bit 0
    bool LightUseLifeSecs : 1;                          // +0x48 bit 1
    bool LightEnabled : 1;                              // +0x48 bit 2
    bool LightRespawns : 1;                             // +0x48 bit 3
    bool LightColourFadeEnable : 1;                     // +0x48 bit 4
    bool LightUseStartColour : 1;                       // +0x48 bit 5
    bool LightUseMidColour : 1;                         // +0x48 bit 6
    bool LightUseEndColour : 1;                         // +0x48 bit 7
    bool LightUseFadeColour : 1;                        // +0x49 bit 0
    bool LightUseTimelineSecs : 1;                      // +0x49 bit 1
    bool StartedTimer : 1;                              // +0x49 bit 2
    CRGBColour LightStartColour;                        // +0x4c
    CRGBColour LightMidColour;                          // +0x50
    CRGBColour LightEndColour;                          // +0x54
    CEnginePrimitiveHandle PrimHandle;                  // +0x58
    CVector3D LightUserPointsArray;                     // +0x60
    void DatabaseSerialise(CPersistContext& context, bool transfer);
};
void CPSCLight::DatabaseSerialise(CPersistContext& context, bool)
{
    {
        bool v = LightEnabled;
        context.Transfer("LightEnabled", v, false);
        LightEnabled = v;
    }
    if (context.IsSaving()) {
        context.Transfer("LightPositionParam", LightPositionParam, 0UL);
    } else {
        CCharString name;
        context.Transfer("LightPositionParam", name, EmptyCCharString());
        LightPositionParam = CRCCalc(0, name.PStringData == 0 ? "" : name.PStringData->Data, name.PStringData == 0 ? 0 : name.PStringData->Length);
    }
    {
        bool v = LightUseLifeSecs;
        context.Transfer("LightUseLifeSecs", v, false);
        LightUseLifeSecs = v;
    }
    context.Transfer("LightLifeSecs", LightLifeSecs, 0.0f);
    {
        bool v = LightRespawns;
        context.Transfer("LightRespawns", v, false);
        LightRespawns = v;
    }
    context.Transfer("LightRespawnDelaySecs", LightRespawnDelaySecs, 0.0f);
    context.Transfer("LightStartRenderWorldRadius", LightStartRenderWorldRadius, 0.0f);
    context.Transfer("LightEndRenderWorldRadius", LightEndRenderWorldRadius, 0.0f);
    context.Transfer("LightAttenuationFactor", LightAttenuationFactor, 0L);
    context.Transfer("LightFadeInEndInteger", LightFadeInEndInteger, 0L);
    context.Transfer("LightFadeOutBeginInteger", LightFadeOutBeginInteger, 0L);
    {
        bool v = LightWorldRadiusFadeEnable;
        context.Transfer("LightWorldRadiusFadeEnable", v, false);
        LightWorldRadiusFadeEnable = v;
    }
    context.Transfer("LightWorldRadiusFadeMinimum", LightWorldRadiusFadeMinimum, 0.0f);
    {
        bool v = LightUseFadeColour;
        context.Transfer("LightUseFadeColour", v, false);
        LightUseFadeColour = v;
    }
    {
        bool v = LightColourFadeEnable;
        context.Transfer("LightColourFadeEnable", v, false);
        LightColourFadeEnable = v;
    }
    context.Transfer("LightColourFadeMinimum", LightColourFadeMinimum, CRGBColour(0, 0, 0, 0xff));
    {
        bool v = LightUseStartColour;
        context.Transfer("LightUseStartColour", v, false);
        LightUseStartColour = v;
    }
    {
        bool v = LightUseMidColour;
        context.Transfer("LightUseMidColour", v, false);
        LightUseMidColour = v;
    }
    {
        bool v = LightUseEndColour;
        context.Transfer("LightUseEndColour", v, false);
        LightUseEndColour = v;
    }
    context.Transfer("LightStartColour", LightStartColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightMidColour", LightMidColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightEndColour", LightEndColour, CRGBColour(0, 0, 0, 0xff));
    {
        bool v = LightUseTimelineSecs;
        context.Transfer("LightUseTimelineSecs", v, false);
        LightUseTimelineSecs = v;
    }
    context.Transfer("LightStartTime", LightStartTime, 0.0f);
    context.Transfer("LightTimelineSecs", LightTimelineSecs, 0.0f);
    long count = LightUserPointsArray.size();
    context.Transfer("LightUserPointsArraySize", count, 0L);
    LightUserPointsArray.resize(count);
    long i = 0;
    if (i < count) {
        C3DVector zero(0.0f, 0.0f, 0.0f);
        do {
            context.Transfer("LightUserPointsArray" + i, LightUserPointsArray[i], zero);
        } while (++i < count);
    }
}
}
