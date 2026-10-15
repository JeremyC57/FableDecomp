// CRegisteredTCInfo::CRegisteredTCInfo(TAllocFunc allocFunc, unsigned long driverType, const CCharString& name) -- record a thing-component factory: its allocator, driver type and copied name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D2EF0, length 35 bytes (`ret 0Ch`). Callees: CCharString::CCharString(const
// CCharString&) @ 0x0099EC30. Called ~330 times from the thing-component registration table builder (0x004EE2B6..).
// Layout: Ego_r.pdb CRegisteredTCInfo (12 bytes): `CTCBase* (__cdecl* AllocFunc)(CThing*)` +0x00, unsigned long DriverType
// +0x04, CCharString Name +0x08; no engine header exists for it.
// Shape: the two scalar initialisers are stored around the argument loads (DriverType's store scheduled first), then the
// Name member's copy constructor runs on `esi+8`.
// Leaf: ??0CRegisteredTCInfo
struct CThing;
struct CTCBase;

struct CCharString {
    void* m_pStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct CRegisteredTCInfo {
    typedef CTCBase* (__cdecl* TAllocFunc)(CThing*);
    TAllocFunc    AllocFunc;                            // +0x00
    unsigned long DriverType;                           // +0x04
    CCharString   Name;                                 // +0x08
    CRegisteredTCInfo(TAllocFunc allocFunc, unsigned long driverType, const CCharString& name);
};

CRegisteredTCInfo::CRegisteredTCInfo(TAllocFunc allocFunc, unsigned long driverType, const CCharString& name)
    : AllocFunc(allocFunc), DriverType(driverType), Name(name)
{
}