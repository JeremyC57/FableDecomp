#pragma optimize("s", on)
// CAbilityTrackLevelDef::CAbilityTrackLevelDef(const CAbilityTrackLevelDef&) -- copy constructor: base CParentDefClassBase(other), own vtable, three dword members +0x3c..+0x44 copied memberwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004787F6, length 45 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (one register for every copy pair).
// Callees: CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) @ 0x0046EE98.
// Vtable 0x01236504 = CAbilityTrackLevelDef (rtti-vtables.tsv). Memberwise one-register copies
// = a user copy constructor. Retail CParentDefClassBase is 0x3c bytes (local model, see 0046f1b4).
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CAbilityTrackLevelDef : public CParentDefClassBase { public:             // vtable 0x01236504
    long m_level;                                                // +0x3c
    long m_experienceCost;                                       // +0x40
    long m_value;                                                // +0x44
    virtual void v00();
    CAbilityTrackLevelDef(const CAbilityTrackLevelDef& other);
};

CAbilityTrackLevelDef::CAbilityTrackLevelDef(const CAbilityTrackLevelDef& other)
    : CParentDefClassBase(other)
    , m_level(other.m_level)
    , m_experienceCost(other.m_experienceCost)
    , m_value(other.m_value)
{
}