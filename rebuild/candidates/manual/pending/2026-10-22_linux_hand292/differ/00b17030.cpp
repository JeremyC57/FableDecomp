// Grade: DIFFER(991v991) -- same size after same-TU auto_inline(off) stand-ins for the unsigned long/long CPersistContext::Transfer overloads (998v991 without); left: the loading-branch CCharString name and its EmptyCCharString default take low frame words (retail: name shares esp+78h with the later bool default), the GetCRC data pointer is chosen straight into edx (retail eax then mov edx,eax), the loop counter/Type-default zero stores come after the 12 default stores (retail first), and the loop head is reached by fall-through padding (retail jmp over lea ecx,[ecx]) with esi based 4 bytes lower.
// NParticleEngine::CPSCOrbit::DatabaseSerialise(CPersistContext& context, bool) -- persist the orbit component by name: the centre param (a CRC when saving, a string hashed back when loading), then for each of the three orbits its settings under "<field><index>" names, the rotate speeds converted to per-second for the transfer and back
// Leaf: DatabaseSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B17030, 992 bytes (`ret 8`).
// Callees: CPersistContext::Transfer(name, unsigned long&, const unsigned long&) 0x004106F0, Transfer(name, CCharString&,
//   const CCharString&) 0x004109A0, Transfer(name, long&, const long&) 0x00410BE0, Transfer<bool> 0x004045C0,
//   Transfer(name, float&, const float&) 0x00410620; CCharString() 0x0099E4B0, copy 0x0099EC30, dtor 0x0099EAE0,
//   operator const char*() 0x0099E4C0; GFIntToCharString(long) 0x0099F830 and operator+(const char*, const CCharString&)
//   0x0099F690 (both by-value returns through ecx); CRC Calc 0x004014A0.
// Data: "CentreParam" 0x012A0474, field names "Type" 0x0125B970 .. "RotateStartRandom" 0x012A03F4, 1/15 0x0126B7DC,
//   15.0f 0x0124E640, empty string 0x0129AAF4.
// Layout: PDB (Ego_r) CPSCOrbit, retail offsets equal the PDB: CentreParam +0x0c, Orbits[3] +0x10 (COrbitSettings, 0x30:
//   Type, Enabled, Radius, Expand, CycleFlag, CycleTime, SqueezeScale, SqueezeAngle, RotateSpeed, RotateStart,
//   RotateSpeedRandom, RotateStartRandom).
struct CCharStringData {
    const char* Data;                                   // +0
    long Length;                                        // +4
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);                                   // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);           // 0x0099F690
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
};
#pragma auto_inline(off)
__declspec(noinline) void CPersistContext::Transfer(const char* name, unsigned long& value, const unsigned long& defaultValue) { if (name == 0) value = defaultValue; }
__declspec(noinline) void CPersistContext::Transfer(const char* name, long& value, const long& defaultValue) { if (name == 0) value = defaultValue; }
#pragma auto_inline(on)
namespace NParticleEngine {
struct COrbitSettings {
    long  Type;                                         // +0x00
    bool  Enabled;                                      // +0x04
    float Radius;                                       // +0x08
    float Expand;                                       // +0x0c
    bool  CycleFlag;                                    // +0x10
    float CycleTime;                                    // +0x14
    float SqueezeScale;                                 // +0x18
    float SqueezeAngle;                                 // +0x1c
    float RotateSpeed;                                  // +0x20
    float RotateStart;                                  // +0x24
    float RotateSpeedRandom;                            // +0x28
    float RotateStartRandom;                            // +0x2c
};
struct CPSCOrbit {
    unsigned char  _base[0xc];                          // CPSCBase
    unsigned long  CentreParam;                         // +0x0c
    COrbitSettings Orbits[3];                           // +0x10
    void DatabaseSerialise(CPersistContext& context, bool transfer);
};
void CPSCOrbit::DatabaseSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        context.Transfer("CentreParam", CentreParam, 0UL);
    } else {
        CCharString name;
        context.Transfer("CentreParam", name, EmptyCCharString());
        CentreParam = GetCRC(name);
    }
    long defaultType = 0;
    bool defaultEnabled = false;
    float defaultRadius = 0.0f;
    float defaultExpand = 0.0f;
    bool defaultCycleFlag = false;
    float defaultCycleTime = 5.0f;
    float defaultSqueezeScale = 0.0f;
    float defaultSqueezeAngle = 0.0f;
    float defaultRotateSpeed = 0.0f;
    float defaultRotateStart = 0.0f;
    float defaultRotateSpeedRandom = 0.0f;
    float defaultRotateStartRandom = 0.0f;
    for (long i = 0; i < 3; ++i) {
        CCharString index = GFIntToCharString(i);
        Orbits[i].RotateSpeed *= 1.0f / 15.0f;
        Orbits[i].RotateSpeedRandom *= 1.0f / 15.0f;
        context.Transfer("Type" + index, Orbits[i].Type, 0L);
        context.Transfer("Enabled" + index, Orbits[i].Enabled, defaultEnabled);
        context.Transfer("Radius" + index, Orbits[i].Radius, defaultRadius);
        context.Transfer((const char*)("Expand" + index), Orbits[i].Expand, defaultExpand);
        context.Transfer((const char*)("CycleFlag" + index), Orbits[i].CycleFlag, defaultCycleFlag);
        context.Transfer((const char*)("CycleTime" + index), Orbits[i].CycleTime, defaultCycleTime);
        context.Transfer("SqueezeScale" + index, Orbits[i].SqueezeScale, defaultSqueezeScale);
        context.Transfer("SqueezeAngle" + index, Orbits[i].SqueezeAngle, defaultSqueezeAngle);
        context.Transfer("RotateSpeed" + index, Orbits[i].RotateSpeed, defaultRotateSpeed);
        context.Transfer("RotateStart" + index, Orbits[i].RotateStart, defaultRotateStart);
        context.Transfer("RotateSpeedRandom" + index, Orbits[i].RotateSpeedRandom, defaultRotateSpeedRandom);
        context.Transfer("RotateStartRandom" + index, Orbits[i].RotateStartRandom, defaultRotateStartRandom);
        Orbits[i].RotateSpeed *= 15.0f;
        Orbits[i].RotateSpeedRandom *= 15.0f;
    }
}
}
