// CCombatActionBase::OnBlowBlocked(bool flag) -- after the creature-action handling, when both the target and the attacker are alive, the attacker's primary-slot weapon (if it carries anything) is alive, and the attacker is in state 2 or its combat type does not ignore blocks, finish this action and make the attacker recoil (CCreatureAction_GeneralInterruptedAttack "STRIKE_RECOIL")
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: ?OnBlowBlocked@CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00858CC0, length 317 bytes (`ret 4`).
// Callees: CCreatureActionBase::OnBlowBlocked(bool) 0x006927F0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::
//          LowerBound const 0x004365B0 (interface 0x46 = CTCCarrying), CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//          CThingCreatureBase::GetCombatTypeDef 0x00666D10, this's slot 4 (+0x10, FinishAction), CCharString 0x0099EBF0,
//          CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase*, CThing*, CCharString) 0x0087CF90 (landed),
//          CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900; literal 0x012764F0.
// Layout: the retail CCombatActionBase (PCreature +8, PTarget +0xa8, 0x114 bytes); the creature's state byte +0x10;
//          the combat type def's block response +0x98.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type) const;  // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char m_state;                       // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];              // +0x20
    CInterfaceMap m_interfaces;                  // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_flags91;                     // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot();             // 0x006A1440
};
class CCombatTypeDef { public:
    unsigned char _pad[0x98];
    long          BlockedResponse;               // +0x98
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
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
class CSoundPair { public: long FirstSound, LastSound; };
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CThingCreatureBase;
class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                 // slot 4 (+0x10)
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;           // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;             // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    virtual ~CCombatActionBase();                        // 0x0062E900
    void CreatureOnBlowBlocked(bool flag);       // 0x006927F0 (CCreatureActionBase::OnBlowBlocked)
    void OnBlowBlocked(bool flag);
};
class CCreatureAction_GeneralInterruptedAttack : public CCombatActionBase { public:   // vtable 0x01277A14
    CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase* owner, CThing* target, CCharString animName);   // 0x0087CF90
};
class CThingCreatureBase : public CThing { public:
    CCombatTypeDef* GetCombatTypeDef();          // 0x00666D10
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

void CCombatActionBase::OnBlowBlocked(bool flag)
{
    CreatureOnBlowBlocked(flag);
    if (IS_THING_ALIVE(PTarget) && IS_THING_ALIVE(PCreature)) {
        bool canRecoil = true;
        CTCCarrying* carrying;
        if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
            CThing* held = carrying->GetThingInPrimarySlot();
            if (!held || (held->m_flags91 & 1))
                canRecoil = false;
        }
        CThing* item = PCreature.GetPItem();
        if ((item->m_state == 2 || ((CThingCreatureBase*)PCreature.GetPItem())->GetCombatTypeDef()->BlockedResponse != 2) && canRecoil) {
            CThing* target = PTarget.GetPItem();
            CThingCreatureBase* creature = (CThingCreatureBase*)PCreature.GetPItem();
            FinishAction();
            creature->SetCurrentAction(CCreatureAction_GeneralInterruptedAttack(creature, target, "STRIKE_RECOIL"));
        }
    }
}