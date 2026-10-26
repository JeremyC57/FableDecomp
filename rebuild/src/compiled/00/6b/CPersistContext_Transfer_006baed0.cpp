// CPersistContext::Transfer(const char* name, CNamedMapPair_006ba420& value) -- the two-argument overload: build a default-constructed value, copy it into the default local, and forward to the three-argument Transfer(name, value, default)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BAED0, length 104 bytes (+ int3 padding).
// Leaf: ?Transfer@CPersistContext@@QAEXPBDAAUCNamedMapPair_006ba420@@@Z
// Label correction: catalogued as Transfer<CCharString>; the value type is a 0x1c-byte record {CCharString, two STLport
// maps} whose default constructor is 0x006BA420 (catalogued "pair<CCharString,CCharString>::pair"), destructor 0x006BA6D0
// (catalogued as the propagated "~CPatchTesselationEdgeStrip") and whose map copy constructor is 0x00514400.
// Callees: the record's ctor @ 0x006BA420 / dtor @ 0x006BA6D0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// the map copy constructor @ 0x00514400 (x2), the three-argument Transfer @ 0x006BABD0 (catalogued
// Transfer<CIntelligentPointer<CThing>>).
// Shape: the default comes from an inline by-value getter whose named local (esp+4) VC7.1 copies (no NRVO) member by
// member through the record's implicit copy constructor into the caller's local (esp+0x20); the getter's local dies
// before the call, the caller's after it (`T def = T();` is elided into one construction); `ret 8`.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();
};
struct CStringMap_00514400 {                       // an STLport map {header, count, compare}
    void*         m_header;
    unsigned long m_count;
    long          m_compare;
    CStringMap_00514400(const CStringMap_00514400& other);   // 0x00514400
    ~CStringMap_00514400();
};
struct CNamedMapPair_006ba420 {                    // 0x1c bytes
    CCharString         Name;                      // +0x00
    CStringMap_00514400 First;                     // +0x04
    CStringMap_00514400 Second;                    // +0x10
    CNamedMapPair_006ba420();                      // 0x006BA420
    ~CNamedMapPair_006ba420();                     // 0x006BA6D0
};

template <class T>
struct CPersistDefault {                           // the default value of a transfer (VC7.1 has no NRVO: the named
    static T Get()                                 // local is built, then copied into the caller's object)
    {
        T value;
        return value;
    }
};

class CPersistContext {
public:
    unsigned char _pad[0x2c];
    void Transfer(const char* name, CNamedMapPair_006ba420& value, const CNamedMapPair_006ba420& defaultValue);   // 0x006BABD0
    void Transfer(const char* name, CNamedMapPair_006ba420& value);
};

void CPersistContext::Transfer(const char* name, CNamedMapPair_006ba420& value)
{
    CNamedMapPair_006ba420 defaultValue = CPersistDefault<CNamedMapPair_006ba420>::Get();
    Transfer(name, value, defaultValue);
}