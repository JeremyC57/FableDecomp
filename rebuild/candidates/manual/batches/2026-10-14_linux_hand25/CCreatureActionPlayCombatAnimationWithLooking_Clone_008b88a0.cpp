// CCreatureAction_PlayCombatAnimationWithLooking::Clone() -- new CCreatureAction_PlayCombatAnimationWithLooking(*this) with the implicit copy constructors inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B88A0, length 89 bytes.
// Callees: operator new @ 0x00BFEA1A (0x118 bytes, null-checked), CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Shape: the base CCreatureAction_PlayCombatAnimation's implicit copy constructor is inlined (base copy out of line, its vtable 0x0127A604,
// the three flag bytes at +0x114..+0x116 copied one register each), then this class's vtable 0x0127A74C replaces it. sizeof = 0x118: no own data.
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {   // vtable 0x0127A604; sizeof 0x118
    unsigned char m_looping;                // +0x114
    unsigned char m_flag;                   // +0x115
    unsigned char m_started;                // +0x116
    virtual void v00();                     // implicit copy constructor
};

struct CCreatureAction_PlayCombatAnimationWithLooking : CCreatureAction_PlayCombatAnimation {   // vtable 0x0127A74C; adds no data
    virtual void v00();
    CCombatActionBase* Clone();
};

CCombatActionBase* CCreatureAction_PlayCombatAnimationWithLooking::Clone()
{
    return new CCreatureAction_PlayCombatAnimationWithLooking(*this);
}
