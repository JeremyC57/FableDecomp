// CModeBase::CModeBase(CThing*) -- creature-mode base ctor: vtable, parent thing, weapon-based CAnimFilterOfGroup, frame countdown -1, inactive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073EAC0, 124 bytes.
// Shape: the vtable (0x012673CC) is stored, the parent thing goes to +4 (scheduled between the
// literal pushes and the CCharString ctor call), then the CAnimFilterOfGroup WeaponBasedFilter at +8
// (vtable 0x01267568) is built from an implicit CCharString conversion temporary of the literal
// @ 0x012674DC: inline ctor = default string (0x0099E4B0), assign (0x0099EFB0), CRC (0x004014A0,
// __fastcall seed/data/length, empty literal 0x0129AAF4 for a null block). The temporary dies
// (0x0099EAE0) [CRC arguments via the GetPString/GetLength ternary accessors; the merged if/else
// form flips the register choice] before FramesBeforeRemovingSelf (+0x14) = -1 and Active (+0x18) = false: plain
// initialisers in declaration order. Members named after the PDB fields listed in the landed
// CModeSleepBed ctor (0x0084B7C0).
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
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = the shared empty literal 0x0129AAF4
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
        m_hash = Calc_004014a0(0, name.GetPString(), name.GetLength());
    }
};

struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                   // +4
    CAnimFilterOfGroup WeaponBasedFilter;  // +8
    long FramesBeforeRemovingSelf;         // +0x14
    bool Active;                           // +0x18
    CModeBase(CThing* thing);
};

CModeBase::CModeBase(CThing* thing)
    : ParentThing(thing)
    , WeaponBasedFilter("WEAPON")          // literal @ 0x012674DC (masked)
    , FramesBeforeRemovingSelf(-1)
    , Active(false)
{
}
