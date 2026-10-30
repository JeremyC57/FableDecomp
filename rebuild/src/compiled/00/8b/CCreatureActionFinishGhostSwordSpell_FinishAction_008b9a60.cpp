// CCreatureAction_FinishGhostSwordSpell::FinishAction() -- base FinishAction, then re-equip the item taken out (fast unsheathe to the combat pose) when it still lives
// Leaf: ?FinishAction@CCreatureAction_FinishGhostSwordSpell@@UAEXXZ
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the +0x91 dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ, a bool accessor) instead of a direct bit test; the bool accessor keeps retail's register
//   choice / test-and-branch shape.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B9A60, 151 bytes, thiscall, plain `ret`.
// Identity (proven): CCreatureAction_FinishGhostSwordSpell vtable 0x0127D14C slot 4 (the FinishAction slot of the landed spell rows); the manifest label
//   agrees.
// Callees: CCombatActionBase::FinishAction() 0x00859A20, CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString(const char*,
//   long) 0x0099EBF0, CCreatureAction_UnsheatheItemFromInventory's constructor 0x0069F0F0 / destructor 0x005CC091,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCharString 0x0099EAE0. Literal 0x0126CCE4 "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE".
// Shape (family K039, representative 0x008B96A0): the landed CCreatureAction_FinishFireballSpell::FinishAction 0x008BAC40 with
//   only its IS_THING_ALIVE(PItemRemoved) re-equip tail (its model, verbatim).
class CCreatureActionBase;
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0 (CThingCreatureBase)
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
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual ~CCreatureActionBase();
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                          // slot 4
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer<CThing> PItemRemoved;             // +0x2c
    unsigned char _pad34[0xa8 - 0x34];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:
    unsigned char _body[0x10];
    CCreatureAction_UnsheatheItemFromInventory(CThing* owner, const CCharString& animation, CThing* item,
                                               long priority, long timeout, CCreatureActionBase* next);   // 0x0069F0F0
    ~CCreatureAction_UnsheatheItemFromInventory();    // 0x005CC091
};
class CCombatActionBase : public CCreatureActionBase { public:      // retail sizeof 0x114
    unsigned char _pad0a8[0x114 - 0xa8];
    virtual void FinishAction();                      // 0x00859A20
};
class CCreatureAction_FinishGhostSwordSpell : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_FinishGhostSwordSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString name("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        GetCreature()->SetCurrentAction(&CCreatureAction_UnsheatheItemFromInventory(GetCreature(), name, PItemRemoved.GetPItem(), 100, 250, 0));
    }
}