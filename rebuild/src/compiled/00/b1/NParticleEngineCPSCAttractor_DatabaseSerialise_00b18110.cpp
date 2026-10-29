// NParticleEngine::CPSCAttractor::DatabaseSerialise(CPersistContext& context, bool) -- persist the attractor settings by name: the two flags, the position param and param name (as CRCs when saving, as strings hashed back to CRCs when loading), radius, force, fall-off type, then the user points array size and each point
// Leaf: DatabaseSerialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18110, 640 bytes (`ret 8`).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer(name, unsigned long&, const unsigned long&) 0x004106F0,
//   Transfer(name, CCharString&, const CCharString&) 0x004109A0, Transfer(name, float&, const float&) 0x00410620,
//   Transfer(name, long&, const long&) 0x00410BE0, Transfer(name, C3DVector&, const C3DVector&) 0x004859AD (labelled
//   Transfer<CFloatRange>, a byte twin); CCharString() 0x0099E4B0, CCharString(const CCharString&) 0x0099EC30,
//   ~CCharString 0x0099EAE0; CRC CCRC::Calc(seed, data, length) 0x004014A0; CArray<C3DVector>::resize 0x0075C040.
// Data: names "AttractorEnabled" .. "AttractorUserPointsArray" 0x012A060C..0x012A052C, empty string 0x0129AAF4.
// Layout: PDB (Ego_r) CPSCAttractor, retail offsets equal the PDB (the point CArray is a 12-byte STLport vector):
//   flag bits +0x0c, AttractorInfluenceFallOff +0x10, AttractorPositionParam +0x14, AttractorPositionParamName +0x18,
//   radius/force +0x1c/+0x20, AttractorUserPointsArray +0x24. CPersistContext Mode +0x18 (saving: 1 or 3).
// Shape: each flag in its own block; loading reads both names into one CCharString with the inline by-value empty
//   default (the landed 0x006E0730 idiom) and an if/else CRC helper (null arm first, one Calc call); the point loop is
//   `if (i < count) { C3DVector zero(0,0,0); do ... while (++i < count); }` so the zero default is stored once inside
//   the guard, in the frame word of the resize default; the point names are the literal advanced by the index (as retail).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CCharStringData { public:
    const char* Data;                                   // +0
    long Length;                                        // +4
};
class CCharString { public:
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
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
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
    return CCRC::Calc(0, data, length);
}

class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          Mode;                                 // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);                       // 0x004045C0
    void Transfer(const char* name, unsigned long& value, const unsigned long& defaultValue);     // 0x004106F0
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);         // 0x004109A0
    void Transfer(const char* name, float& value, const float& defaultValue);                     // 0x00410620
    void Transfer(const char* name, long& value, const long& defaultValue);                       // 0x00410BE0
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
namespace NParticleEngine {
class CPSCAttractor { public:
    unsigned char _base[0xc];                           // CPSCBase
    bool AttractorEnabled : 1;                          // +0x0c bit 0
    bool AttractorUseParamPosition : 1;                 // +0x0c bit 1
    long AttractorInfluenceFallOff;                     // +0x10
    unsigned long AttractorPositionParam;               // +0x14
    unsigned long AttractorPositionParamName;           // +0x18
    float AttractorInfluenceRadius;                     // +0x1c
    float AttractorInfluenceForce;                      // +0x20
    CVector3D AttractorUserPointsArray;                 // +0x24
    void DatabaseSerialise(CPersistContext& context, bool transfer);
};
void CPSCAttractor::DatabaseSerialise(CPersistContext& context, bool)
{
    {
        bool enabled = AttractorEnabled;
        context.Transfer("AttractorEnabled", enabled, false);
        AttractorEnabled = enabled;
    }
    {
        bool useParam = AttractorUseParamPosition;
        context.Transfer("AttractorUseParamPosition", useParam, false);
        AttractorUseParamPosition = useParam;
    }
    if (context.IsSaving()) {
        context.Transfer("AttractorPositionParam", AttractorPositionParam, 0UL);
        context.Transfer("AttractorPositionParamName", AttractorPositionParamName, 0UL);
    } else {
        CCharString name;
        context.Transfer("AttractorPositionParam", name, EmptyCCharString());
        AttractorPositionParam = GetCRC(name);
        context.Transfer("AttractorPositionParamName", name, EmptyCCharString());
        AttractorPositionParamName = GetCRC(name);
    }
    context.Transfer("AttractorInfluenceRadius", AttractorInfluenceRadius, 0.0f);
    context.Transfer("AttractorInfluenceForce", AttractorInfluenceForce, 0.0f);
    context.Transfer("AttractorInfluenceFallOffType", AttractorInfluenceFallOff, 0L);
    long count = AttractorUserPointsArray.size();
    context.Transfer("AttractorUserPointsArraySize", count, 0L);
    AttractorUserPointsArray.resize(count);
    long i = 0;
    if (i < count) {
        C3DVector zero(0.0f, 0.0f, 0.0f);
        do {
            context.Transfer("AttractorUserPointsArray" + i, AttractorUserPointsArray[i], zero);
        } while (++i < count);
    }
}
}