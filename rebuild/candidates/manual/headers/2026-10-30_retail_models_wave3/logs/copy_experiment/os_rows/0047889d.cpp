#pragma optimize("s", on)
// CAbilityTrackDef::operator=(const CAbilityTrackDef&) -- assignment: base CParentDefClassBase::operator=, a dword at +0x3c, the vector<CAbilityTrackLevelDef> at +0x40 by its operator=, a dword at +0x4c; returns *this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047889D, length 45 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (one register for every copy pair).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<CAbilityTrackLevelDef>::operator= @ 0x004788CA.
// The compiler-generated memberwise assignment for the class of 004786cf: base assignment out of
// line, then each member in declaration order (the retail vector's operator= is out of line).
// Written as a user operator= with the same order; retail CParentDefClassBase is 0x3c bytes.
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#include "/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/mig3/work/x/chain.h"

struct CAbilityTrackLevelDefVector {                             // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CAbilityTrackLevelDefVector& operator=(const CAbilityTrackLevelDefVector& other);   // 0x004788CA
};

class CAbilityTrackDef : public CParentDefClassBase { public:                  // vtable 0x0123649C
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