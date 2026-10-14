#pragma optimize("s", on)
// CAbilityTrackDef::operator=(const CAbilityTrackDef&) -- assignment: base CParentDefClassBase::operator=, a dword at +0x3c, the vector<CAbilityTrackLevelDef> at +0x40 by its operator=, a dword at +0x4c; returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047889D, length 45 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (one register for every copy pair).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<CAbilityTrackLevelDef>::operator= @ 0x004788CA.
// The compiler-generated memberwise assignment for the class of 004786cf: base assignment out of
// line, then each member in declaration order (the retail vector's operator= is out of line).
// Written as a user operator= with the same order; retail CParentDefClassBase is 0x3c bytes.
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                   // +0x04..+0x3c
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};

struct CAbilityTrackLevelDefVector {                             // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CAbilityTrackLevelDefVector& operator=(const CAbilityTrackLevelDefVector& other);   // 0x004788CA
};

struct CAbilityTrackDef : CParentDefClassBase {                  // vtable 0x0123649C
    long m_abilityType;                                          // +0x3c
    CAbilityTrackLevelDefVector m_levels;                        // +0x40
    long m_maxLevel;                                             // +0x4c
    virtual void v00();
    CAbilityTrackDef& operator=(const CAbilityTrackDef& other);
};

CAbilityTrackDef& CAbilityTrackDef::operator=(const CAbilityTrackDef& other)
{
    CParentDefClassBase::operator=(other);
    m_abilityType = other.m_abilityType;
    m_levels = other.m_levels;
    m_maxLevel = other.m_maxLevel;
    return *this;
}