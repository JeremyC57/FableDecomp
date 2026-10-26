// CCombatAction_GenericStrikeResponseBase::IsActionReadyForInterruption() const -- ready when the creature is in state 2 and the response flag is set, or in state 1 within the first 6 running frames; else the base decides
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00865CF0, length 52 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on PCreature at +8; its address is formed once
// into edi), CCombatActionBase::IsActionReadyForInterruption @ 0x00856170 (the base virtual called
// non-virtually as the tail `jmp`).
// Reads: the creature's byte at +0x10 (its combat state), this+0xdc (a bool of CCombatActionBase, retail
// layout; name descriptive), TotalTime (+0x10) and FramesRunning (+0x1c) from CCreatureActionBase (Ego_r.pdb names).
// Shape: ONE `(A && B) || (C && D)` condition returning true (single `mov al,1` tail reached by `jne` from the
// first clause); written as two separate ifs VC hoists `mov al,1` and compares the state against al.
// No engine header exists for this class; CCombatActionBase retail sizeof is 0x114 (the PDB build's is 0x12c).
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
    long TotalTime;                             // +0x10
    long ActionTime;                            // +0x14
    long AnimDelayTime;                         // +0x18
    long FramesRunning;                         // +0x1c
    unsigned char _pad20[0xdc - 0x20];          // +0x20
    bool m_respondsInState2;                    // +0xdc
    unsigned char _paddd[0x114 - 0xdd];         // +0xdd
    bool IsActionReadyForInterruption() const;  // 0x00856170 (base implementation)
};

class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase { public:
    bool IsActionReadyForInterruption() const;
};

bool CCombatAction_GenericStrikeResponseBase::IsActionReadyForInterruption() const
{
    if ((PCreature.GetPItem()->CombatState == 2 && m_respondsInState2)
        || (PCreature.GetPItem()->CombatState == 1 && TotalTime - FramesRunning < 6))
        return true;
    return CCombatActionBase::IsActionReadyForInterruption();
}