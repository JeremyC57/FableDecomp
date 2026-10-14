// CCreatureAction_UndeadRiseUp::Clone() -- new CCreatureAction_UndeadRiseUp(*this) with two levels of implicit copy constructor inlined: the CCombatActionBase copy out of line, the CCreatureAction_RiseUp vtable (0x01268C9C) and its byte at +0x114, then this class's vtable (0x01268F44) and its byte at +0x118
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074DF80, length 80 bytes.
// Callees: operator new @ 0x00BFEA1A (0x11c bytes, null-checked), CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Shape: the landed Clone family (0x008C3D60); both class names come from rtti-vtables.tsv. RiseUp's byte is padded to +0x118 so UndeadRiseUp's byte
// follows it; sizeof = 0x11c.
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCreatureAction_RiseUp : CCombatActionBase {   // vtable 0x01268C9C; sizeof 0x118
    unsigned char m_isUnderground;          // +0x114
    virtual void v00();                     // implicit copy constructor
};

struct CCreatureAction_UndeadRiseUp : CCreatureAction_RiseUp {   // vtable 0x01268F44; sizeof 0x11c
    unsigned char m_isSuspended;            // +0x118
    virtual void v00();                     // implicit copy constructor
    CCombatActionBase* Clone();
};

CCombatActionBase* CCreatureAction_UndeadRiseUp::Clone()
{
    return new CCreatureAction_UndeadRiseUp(*this);
}
