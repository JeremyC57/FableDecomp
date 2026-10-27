// CCombatAction_GenericStrikeResponseBase::IsAlwaysPhysicsPushableBySelfAvoidance() const -- pushable while the action drives the movement
// Leaf: ?IsAlwaysPhysicsPushableBySelfAvoidance@CCombatAction_GenericStrikeResponseBase@@UBE_NXZ
// Retail: Fable.exe @ 0x00865B50, 11 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): vtable slot 33 (+0x84) of CCombatAction_GenericStrikeResponseBase 0x01264734 (read from Fable.exe;
// inherited by CCombatAction_StrikeResponseMazeDoForcePush 0x012625E4); slot 33 of CCreatureActionBase 0x01259D24 is the
// landed CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance (0x00692750), and Ego_r.pdb has
// ?IsAlwaysPhysicsPushableBySelfAvoidance@CCombatAction_GenericStrikeResponseBase@@UBE_NXZ.
// Callees: 0x00692A50 ?IsUsingActionMovement@CCreatureActionBase@@QBE_NXZ (Ego_r.pdb: public, const, non-virtual;
// reloc_bindings carries the stand-in ?IsUsingActionMovement@CCombatActionBase@@QAE_NXZ).
// Shape: `if (!f()) return false; return true;` over the bool callee gives `test al,al ; setne al`.
// Layout: no member access; a minimal polymorphic chain CCreatureActionBase -> CCombatActionBase -> this class (RTTI).
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    bool IsUsingActionMovement() const;                                    // 0x00692A50
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;
};

class CCombatActionBase : public CCreatureActionBase {
};

class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase {
public:
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;
};

bool CCombatAction_GenericStrikeResponseBase::IsAlwaysPhysicsPushableBySelfAvoidance() const
{
    if (!IsUsingActionMovement())
        return false;
    return true;
}