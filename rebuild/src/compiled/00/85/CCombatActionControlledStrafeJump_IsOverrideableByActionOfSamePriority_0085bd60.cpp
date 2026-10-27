// CCombatAction_ControlledStrafeJump::IsOverrideableByActionOfSamePriority() const -- overrideable once the jump is ready for interruption
// Leaf: ?IsOverrideableByActionOfSamePriority@CCombatAction_ControlledStrafeJump@@UBE_NXZ
// Retail: Fable.exe @ 0x0085BD60, 11 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): vtable slot 24 (+0x60) of CCombatAction_ControlledStrafeJump 0x01276974; slot 24 of the
// CCreatureActionBase vtable 0x01259D24 is the landed CCreatureActionBase::IsOverrideableByActionOfSamePriority
// (0x00692540), and Ego_r.pdb has ?IsOverrideableByActionOfSamePriority@CCombatAction_ControlledStrafeJump@@UBE_NXZ.
// Callees: virtual slot 27 (+0x6C) IsActionReadyForInterruption (CCreatureActionBase 0x00692570, overridden by
// CCombatActionBase 0x00856170; Ego_r.pdb ?IsActionReadyForInterruption@CCreatureActionBase@@UBE_NXZ).
// Shape: `if (!f()) return false; return true;` over the bool virtual gives `test al,al ; setne al`.
// Layout: engine/CCombatAction_ControlledStrafeJump.h / CCreatureActionBase.h carry the vptr as a data word, so a virtual
// call cannot go through them: local polymorphic model with placeholder slots 0..23 and the real names of slots 24..27
// (from the CCreatureActionBase vtable). Header debt (NOTES.md).
#define V4(n) virtual void _slot##n##a(); virtual void _slot##n##b(); virtual void _slot##n##c(); virtual void _slot##n##d();

class CCreatureActionBase {
public:
    V4(0) V4(1) V4(2) V4(3) V4(4) V4(5)                                   // slots 0..23
    virtual bool IsOverrideableByActionOfSamePriority() const;             // slot 24 (+0x60)
    virtual void OnFrameUpdate();                                          // slot 25
    virtual bool IsQueuable() const;                                       // slot 26
    virtual bool IsActionReadyForInterruption() const;                     // slot 27 (+0x6C)
};

class CCombatActionBase : public CCreatureActionBase {
};

class CCombatAction_ControlledStrafeJump : public CCombatActionBase {
public:
    virtual bool IsOverrideableByActionOfSamePriority() const;
};

bool CCombatAction_ControlledStrafeJump::IsOverrideableByActionOfSamePriority() const
{
    if (!IsActionReadyForInterruption())
        return false;
    return true;
}