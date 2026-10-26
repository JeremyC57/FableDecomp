// NCreatureMode::CModeCreatureIsProjectile::CModeCreatureIsProjectile(CThing*) -- creature-mode ctor: filter at +0x74, owner kept at +0x84
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D7D60.
// The owner argument stays in ebx across the base ctor and lands at +0x84 after the temporary dies (a plain initialiser).
// Shared shape (same family as the landed CModeBerserk ctor @ 0x007409C0): the base constructor
// @ 0x00861E00 runs with the owner, the class vtable (0x01285728) is installed, then the
// CAnimFilterOfGroup member at +0x74 (vtable 0x01267568) is built from an implicit CCharString
// conversion temporary of the literal @ 0x01285758: its inline ctor default-constructs the string
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
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

struct CModeBase_00861e00 {
    virtual void v00();
    unsigned char _body[0x64];
    CModeBase_00861e00(CThing* thing);     // 0x00861E00
};

namespace NCreatureMode {

struct CModeCreatureIsProjectile : CModeBase_00861e00 {
    unsigned char m_state[0xc];            // +0x68 (not touched by the ctor)
    CAnimFilterOfGroup m_animFilter;       // +0x74
    unsigned long OldTargetFlags;             // +0x80 (not touched by the ctor)
    CThing* m_pThrower;                    // +0x84
    virtual void v00();
    CModeCreatureIsProjectile(CThing* thing);
};

CModeCreatureIsProjectile::CModeCreatureIsProjectile(CThing* thing)
    : CModeBase_00861e00(thing)
    , m_animFilter("PROJECTILE")           // literal @ 0x01285758 (masked)
    , m_pThrower(thing)
{
}

}