// CTCBase::CRegisteredTCInfo::CRegisteredTCInfo(const CRegisteredTCInfo& other) -- member-wise copy constructor
// Leaf: ??0CRegisteredTCInfo@CTCBase@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6C66, 35 bytes, thiscall, `ret 4`, returns this (size-optimised module).
// Identity (proven): bound ??0CRegisteredTCInfo@CTCBase@@QAE@ABV01@@Z; its callers are the pair<CCharString, CTCBase::CRegisteredTCInfo> constructors 0x004D788D / 0x004D9D2F (family M021's 0x004D9D2F); Ego_r.pdb {AllocFunc, DriverType, CCharString Name}.
// Callees: the third member's out-of-line copy 0x0099EC30.
// Shape (family M096, representative 0x004D6C66): the two leading dwords copied through ecx, then the third member's copy
//   with `lea ecx,[esi+8]` and the source + 8 pushed; `optimize("s")` keeps the third member's copy out of line.
#pragma optimize("s", on)
class CThing;
class CTCBase;
class CCharString {                                       // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
    void* PStringData;
};
class CTCBase {
public:
    class CRegisteredTCInfo {                             // Ego_r.pdb, 12 bytes
    public:
        CRegisteredTCInfo(const CRegisteredTCInfo& other);
        CRegisteredTCInfo& operator=(const CRegisteredTCInfo& other);
        CTCBase* (__fastcall* AllocFunc)(CThing& thing);  // +0x0
        unsigned long DriverType;                         // +0x4
        CCharString Name;                                 // +0x8
    };
};
CTCBase::CRegisteredTCInfo::CRegisteredTCInfo(const CRegisteredTCInfo& other)
    : AllocFunc(other.AllocFunc), DriverType(other.DriverType), Name(other.Name)
{
}