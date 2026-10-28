// NCreatureMode::CModeChargeForcePush::CModeChargeForcePush(CThing*) -- base mode ctor, own vtable, CAnimFilterOfGroup "charge" filter at +0x68, flag at +0x74 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00741730, 128 bytes (padding included).
// Shape (the CRC arguments come from the CCharString GetPString/GetLength ternary accessors, as in the
// out-of-line CAnimFilterOfGroup ctor 0x0073E8F0; the merged if/else form flips the register choice
// when constant stores follow): the shared creature-mode base constructor @ 0x00861E00 runs with the owner, this class's
// vtable (0x01268328, rtti-vtables.tsv NCreatureMode::CModeChargeForcePush) is installed, then the
// CAnimFilterOfGroup member at +0x68 (vtable 0x01267568) is built from an implicit CCharString
// conversion temporary of the literal @ 0x01268358: its inline ctor default-constructs the string
// (0x0099E4B0), assigns the name (0x0099EFB0) and stores the name's CRC (0x004014A0, __fastcall
// seed/data/length; the empty literal 0x0129AAF4 when the string has no data block). The
// temporary dies (0x0099EAE0) and only then the bool at +0x74 is cleared (the next initialiser). Same family as the
// landed CModeBerserk ctor @ 0x007409C0.
class CThing;

class CCharStringData { public:
    const char* m_pData;                   // +0
    unsigned long m_length;                // +4
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = the shared empty literal 0x0129AAF4
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:
    virtual void v00();
    CCharString m_string;                  // +4
    unsigned long m_hash;                  // +8
    CAnimFilterOfGroup(const CCharString& name)
        : m_string()
    {
        m_string = name;
        m_hash = CCRC::Calc(0, name.GetPString(), name.GetLength());
    }
};

struct CModeBase_00861e00 {
    virtual void v00();
    unsigned char _body[0x64];
    CModeBase_00861e00(CThing* thing);     // 0x00861E00
};

struct CModeChargeForcePush : CModeBase_00861e00 {
    CAnimFilterOfGroup m_animFilter;       // +0x68
    bool m_pushed;                         // +0x74
    virtual void v00();
    CModeChargeForcePush(CThing* thing);
};

CModeChargeForcePush::CModeChargeForcePush(CThing* thing)
    : CModeBase_00861e00(thing)
    , m_animFilter("CHARGE")               // literal @ 0x01268358 (text masked by the grader)
    , m_pushed(false)
{
}