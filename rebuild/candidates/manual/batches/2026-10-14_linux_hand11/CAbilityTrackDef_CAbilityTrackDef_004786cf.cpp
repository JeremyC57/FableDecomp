#pragma optimize("s", on)
// CAbilityTrackDef::CAbilityTrackDef(const CAbilityTrackDef&) -- copy constructor: base CParentDefClassBase(other), own vtable, a dword at +0x3c, a vector<CAbilityTrackLevelDef> at +0x40 (out-of-line copy ctor), a dword at +0x4c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004786CF, length 51 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (one register for every copy pair).
// Callees: CParentDefClassBase::CParentDefClassBase(const&) @ 0x0046EE98,
//          vector<CAbilityTrackLevelDef>::vector(const vector&) @ 0x0047876B (12-byte retail vector at +0x40).
// Vtable 0x0123649C = CAbilityTrackDef (rtti-vtables.tsv). Members copied in declaration order:
// the dword before the vector, the vector by its copy constructor (this = &m_levels, the source
// member's address pushed), then the dword after it. Retail CParentDefClassBase is 0x3c bytes
// (local model, see 0046f1b4).
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                   // +0x04..+0x3c
    CParentDefClassBase(const CParentDefClassBase& other);       // 0x0046EE98
};

struct CAbilityTrackLevelDefVector {                             // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CAbilityTrackLevelDefVector(const CAbilityTrackLevelDefVector& other);   // 0x0047876B
};

struct CAbilityTrackDef : CParentDefClassBase {                  // vtable 0x0123649C
    long m_abilityType;                                          // +0x3c
    CAbilityTrackLevelDefVector m_levels;                        // +0x40
    long m_maxLevel;                                             // +0x4c
    virtual void v00();
    CAbilityTrackDef(const CAbilityTrackDef& other);
};

CAbilityTrackDef::CAbilityTrackDef(const CAbilityTrackDef& other)
    : CParentDefClassBase(other)
    , m_abilityType(other.m_abilityType)
    , m_levels(other.m_levels)
    , m_maxLevel(other.m_maxLevel)
{
}
