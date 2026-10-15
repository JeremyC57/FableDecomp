// NCreatureMode::CModeChargeForcePush::CModeChargeForcePush(CThing*, bool) -- base mode ctor, own vtable, CAnimFilterOfGroup filter at +0x68, flag at +0x74 from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BD7F0, 123 bytes.
// Shape: the shared creature-mode base constructor @ 0x00861E00 runs with the owner, this class's
// vtable (0x01268328, rtti-vtables.tsv NCreatureMode::CModeChargeForcePush) is installed, then the
// CAnimFilterOfGroup member at +0x68 (vtable 0x01267568) is built from an implicit CCharString
// conversion temporary of the literal @ 0x01268358: its inline ctor default-constructs the string
// (0x0099E4B0), assigns the name (0x0099EFB0) and stores the CRC (0x004014A0, __fastcall
// seed/data/length; the empty literal 0x0129AAF4 when the string has no block). The temporary dies
// (0x0099EAE0) before the bool argument is stored at +0x74 (initialiser after the filter member).
// Twin of the one-argument ctor @ 0x00741730 (parked: register allocation differs there).
struct CThing;

struct CCharStringData {
    const char* m_pData;                   // +0
    unsigned long m_length;                // +4
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {
    virtual void v00();
    CCharString m_string;                  // +4
    unsigned long m_hash;                  // +8
    CAnimFilterOfGroup(const CCharString& name)
        : m_string()
    {
        m_string = name;
        const char* data;
        unsigned long length;
        if (name.m_pData == 0) {
            length = 0;
            data = "";
        }
        else {
            length = name.m_pData->m_length;
            data = name.m_pData->m_pData;
        }
        m_hash = Calc_004014a0(0, data, length);
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
    CModeChargeForcePush(CThing* thing, bool pushed);
};

CModeChargeForcePush::CModeChargeForcePush(CThing* thing, bool pushed)
    : CModeBase_00861e00(thing)
    , m_animFilter("CHARGE")               // literal @ 0x01268358 (text masked by the grader)
    , m_pushed(pushed)
{
}