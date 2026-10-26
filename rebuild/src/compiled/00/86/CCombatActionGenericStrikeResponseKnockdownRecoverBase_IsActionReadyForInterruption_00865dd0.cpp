// CCombatAction_GenericStrikeResponseKnockdownRecoverBase::IsActionReadyForInterruption() const -- ready when the creature is in combat state 2 and the response flag at +0xdc is set, else the base decides
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00865DD0, length 39 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CCombatActionBase::
// IsActionReadyForInterruption @ 0x00856170 (base virtual called non-virtually; tail `jmp`).
// The first clause of the sibling override @ 0x00865CF0 (CCombatAction_GenericStrikeResponseBase) without the
// frame-window clause. Engine header CCombatAction_GenericStrikeResponseKnockdownRecoverBase.h describes the
// PDB build (base 0x12c bytes v retail 0x114), so the layout is modelled locally with descriptive names.
class CThingCreatureBase_Retail { public:              // engine/CThingCreatureBase.h exists but is not used: only the combat-state byte at +0x10 is read (retail offset)
    unsigned char _pad[0x10];
    unsigned char CombatState;                  // +0x10
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CIntelligentPointer<CThingCreatureBase_Retail> PCreature;          // +0x08
    unsigned char _pad10[0xdc - 0x10];          // +0x10
    bool m_respondsInState2;                    // +0xdc
    unsigned char _paddd[0x114 - 0xdd];         // +0xdd
    bool IsActionReadyForInterruption() const;  // 0x00856170 (base implementation)
};

class CCombatAction_GenericStrikeResponseKnockdownRecoverBase : public CCombatActionBase { public:
    bool IsActionReadyForInterruption() const;
};

bool CCombatAction_GenericStrikeResponseKnockdownRecoverBase::IsActionReadyForInterruption() const
{
    if (PCreature.GetPItem()->CombatState == 2) {
        if (m_respondsInState2)
            return true;
    }
    return CCombatActionBase::IsActionReadyForInterruption();
}