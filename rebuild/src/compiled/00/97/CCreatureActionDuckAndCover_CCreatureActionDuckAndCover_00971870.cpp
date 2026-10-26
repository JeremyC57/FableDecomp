// CCreatureAction_DuckAndCover::CCreatureAction_DuckAndCover(const CCreatureAction_DuckAndCover&) -- copy constructor: base CCombatActionBase(other), own vtable, three dword members +0x114..+0x11c copied memberwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00971870, length 63 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// The manifest label (CAbilityTrackLevelDef) is propagated: the vtable stored, 0x012974A4, is
// CCreatureAction_DuckAndCover in rtti-vtables.tsv. The base is 0x114 bytes here; the three
// members after it are copied one register each (a user copy constructor's memberwise shape).
class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x110];                                  // +0x04..+0x114
    CCombatActionBase(const CCombatActionBase& other);           // 0x0062F3B0
};

class CCreatureAction_DuckAndCover : public CCombatActionBase { public:        // vtable 0x012974A4
    long m_coverState;                                           // +0x114
    long m_coverTimer;                                           // +0x118
    long m_coverTarget;                                          // +0x11c
    virtual void v00();
    CCreatureAction_DuckAndCover(const CCreatureAction_DuckAndCover& other);
};

CCreatureAction_DuckAndCover::CCreatureAction_DuckAndCover(const CCreatureAction_DuckAndCover& other)
    : CCombatActionBase(other)
    , m_coverState(other.m_coverState)
    , m_coverTimer(other.m_coverTimer)
    , m_coverTarget(other.m_coverTarget)
{
}