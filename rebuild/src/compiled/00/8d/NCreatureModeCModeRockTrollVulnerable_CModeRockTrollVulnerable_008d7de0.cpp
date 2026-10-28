// NCreatureMode::CModeRockTrollVulnerable::CModeRockTrollVulnerable(CThing*) -- creature-mode ctor over CModeBase: filter at +0x20, owner at +0x2c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D7DE0.
// Leaf: CModeRockTrollVulnerable
// Label correction: the catalog row says CModeCreatureIsProjectile, but the vtable store 0x01285774 is NCreatureMode::CModeRockTrollVulnerable (rtti-vtables.tsv); the base is CModeBase @ 0x0073EAC0.
// Shared shape (same family as the landed CModeBerserk ctor @ 0x007409C0): the base constructor
// @ 0x0073EAC0 runs with the owner, the class vtable (0x01285774) is installed, then the
// CAnimFilterOfGroup member at +0x20 (vtable 0x01267568) is built from an implicit CCharString
// conversion temporary of the literal @ 0x0122D70E: its inline ctor default-constructs the string
// (0x0099E4B0), assigns the name (0x0099EFB0) and stores the CRC (0x004014A0, __fastcall
// seed/data/length; the empty literal 0x0129AAF4 when the string has no block). The temporary dies
// (0x0099EAE0) before the remaining initialisers run.
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
        m_hash = CCRC::Calc(0, data, length);
    }
};

struct CModeBase {
    virtual void v00();
    unsigned char _body[0x18];
    CModeBase(CThing* thing);     // 0x0073EAC0
};

namespace NCreatureMode {

struct CModeRockTrollVulnerable : CModeBase {
    unsigned long RecoveryCount;             // +0x1c (not touched by the ctor)
    CAnimFilterOfGroup m_animFilter;       // +0x20
    CThing* m_pOwner;                      // +0x2c
    virtual void v00();
    CModeRockTrollVulnerable(CThing* thing);
};

CModeRockTrollVulnerable::CModeRockTrollVulnerable(CThing* thing)
    : CModeBase(thing)
    , m_animFilter("VULNERABLE")           // literal @ 0x0122D70E (masked)
    , m_pOwner(thing)
{
}

}