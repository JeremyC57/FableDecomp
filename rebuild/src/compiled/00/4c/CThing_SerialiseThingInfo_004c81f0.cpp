// CThing::SerialiseThingInfo(CPersistContext& context, CThing::CSerialiseInfo& info) -- persist a thing's serialise header by name: "Player" (default 0), "UID" (default 0), "DefinitionType" (default empty string)
// Leaf: ?SerialiseThingInfo@CThing@@SIXAAVCPersistContext@@AAVCSerialiseInfo@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C81F0, length 144 bytes (137 + int3 padding), __fastcall (ecx = context, edx = info), plain `ret`.
// Identity: Ego_r.pdb `?SerialiseThingInfo@CThing@@SIXAAVCPersistContext@@AAVCSerialiseInfo@1@@Z` (static __fastcall);
// callers CThing::Save 0x004C9120, CThingManager::LoadSingleThing 0x0051FD80 and the bool/CCharString overload 0x004C88E0.
// Callees: CPersistContext::Transfer(name, signed char&, const signed char&) @ 0x004CC5C0, Transfer(name, unsigned __int64&,
// const unsigned __int64&) @ 0x0044BB00, Transfer(name, CCharString&, const CCharString&) @ 0x004109A0 (landed names);
// CCharString() @ 0x0099E4B0, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0.
// Data: "Player" 0x01239AB0, "UID" 0x01239AAC, "DefinitionType" 0x01239A9C.
// Layout (Ego_r.pdb CThing::CSerialiseInfo, 16 bytes): UID +0 (unsigned __int64), DefinitionType +8 (CCharString),
// Player +0xc (signed char).
// Shape: the string default is the by-value empty CCharString (built, copied into the argument temporary, the source
// destroyed before the call; the landed 0x00B18110 / 0x006E0730 idiom); the defaults share one 16-byte frame.
class CCharStringData;
class CCharString {
public:
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

class CPersistContext {
public:
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);             // 0x004CC5C0
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);             // 0x004109A0
};

class CThing {
public:
    class CSerialiseInfo {                              // Ego_r.pdb, 16 bytes
    public:
        unsigned __int64 UID;                           // +0x00
        CCharString      DefinitionType;                // +0x08
        signed char      Player;                        // +0x0c
    };
    static void __fastcall SerialiseThingInfo(CPersistContext& context, CSerialiseInfo& info);
};

void __fastcall CThing::SerialiseThingInfo(CPersistContext& context, CSerialiseInfo& info)
{
    context.Transfer("Player", info.Player, (signed char)0);
    context.Transfer("UID", info.UID, (unsigned __int64)0);
    context.Transfer("DefinitionType", info.DefinitionType, EmptyCCharString());
}