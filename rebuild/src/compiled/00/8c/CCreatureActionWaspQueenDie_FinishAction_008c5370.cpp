// CCreatureAction_WaspQueenDie::FinishAction() -- base FinishAction; count the death loop: on the fifth loop kill the creature, otherwise restart the action with a fresh CCreatureAction_WaspQueenDie(creature, LoopNum)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C5370, length 158 bytes.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8), CThing::Kill(bool) @ 0x004C9B80, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long,
// const CSoundPair&) @ 0x00858030 (action code 250), CThingCreatureBase::SetCurrentAction @ 0x006644F0, the action
// destructor @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the restart builds this very class as a 0x118-byte temporary (inline constructor: base, vtable 0x01280D84, then
// LoopNum at +0x114 from the argument) above the zeroed 8-byte CSoundPair; the early `return` duplicates the epilogue.
// The creature is a named local fetched before the temporary, so LoopNum is re-read after that call (retail
// `mov esi,[esi+114h]`) and the incremented value is not kept in a callee-saved register (`mov eax,ecx ; cmp eax,5`).
// LoopNum (Ego_r.pdb +0x12c) is +0x114 in retail (CCombatActionBase is 0x114 bytes); modelled locally.
class CThingCreatureBase;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    virtual ~CCombatActionBase();                       // 0x0062E900
    void FinishAction();                        // 0x00859A20
};

class CThingCreatureBase { public:
    void Kill(bool b);                                        // 0x004C9B80 (CThing)
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

class CCreatureAction_WaspQueenDie : public CCombatActionBase { public:   // retail sizeof 0x118, vtable 0x01280D84
    long LoopNum;                               // +0x114
    virtual void v00();
    CCreatureAction_WaspQueenDie(CThingCreatureBase* owner, long loopNum)
        : CCombatActionBase(owner, 250, CSoundPair())
        , LoopNum(loopNum)
    {
    }
    void FinishAction();
};

void CCreatureAction_WaspQueenDie::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (++LoopNum == 5) {
        PCreature.GetPItem()->Kill(true);
        return;
    }
    CThingCreatureBase* creature = PCreature.GetPItem();   // fetched first: LoopNum is re-read after this call
    PCreature.GetPItem()->SetCurrentAction(CCreatureAction_WaspQueenDie(creature, LoopNum));
}