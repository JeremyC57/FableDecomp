// CCreatureAction_BanditKingGetHit::FinishAction() -- base FinishAction; when the attacker is alive and flagged (+0x6c bit 3), answer with a CCreatureAction_BanditKingAttack: "REAR_ATTACK" (0, 1) when hit from behind, else "STRIKE_MEDIUM_FRONT" (1, 0)
// Leaf: FinishAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C92C0, 235 bytes (240 with padding), virtual FinishAction.
// Callees: CCombatAction_GenericStrikeResponseBase::FinishAction 0x008663D0, CBaseIntelligentPointer::GetPItem 0x00A01B50, the __fastcall hit-from-behind
//   test 0x008C7E50 (manifest "CompareActorRotations") (creature in ecx, attacker in edx), CCharString::CCharString(const char*, int = -1) 0x0099EBF0
//   ("REAR_ATTACK" 0x01282028 / "STRIKE_MEDIUM_FRONT" 0x012778B0), CCreatureAction_BanditKingAttack ctor 0x008C9410,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900, ~CCharString 0x0099EAE0.
// Layout: retail CCombatActionBase is 0x114 bytes (PCreature +0x08); the attacker pointer is this class's +0x114; the
//   attacker's flag byte +0x6c.
// Shape: each branch builds a named animation string, then fetches the action's target creature into a named local,
//   then the SetCurrentAction statement; the tails (attacker, owner, ctor, call, both
//   destructors) the compiler merges; the attacker and creature are named locals fetched before the test.
class CThing { public:
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                    // +0x6c bit 3
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};

class CCombatActionBase;

class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, int length = -1);   // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCreaturePointer {                       // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CThingCreatureBase* m_pItem;                // +4
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

bool __fastcall IsHitFromBehind(CThingCreatureBase* creature, CThing* attacker);   // 0x008C7E50

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CCreaturePointer PCreature;                 // +0x08
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    ~CCombatActionBase();                       // 0x0062E900
};

class CCreatureAction_BanditKingAttack : public CCombatActionBase { public:   // retail sizeof 0x118
    unsigned char _body[4];
    CCreatureAction_BanditKingAttack(CThingCreatureBase* owner, CThing* target, const CCharString& animation, long first, long second);   // 0x008C9410
};

class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase { public:
    void FinishAction();                        // 0x008663D0
};

class CCreatureAction_BanditKingGetHit : public CCombatAction_GenericStrikeResponseBase { public:
    CBaseIntelligentPointer PAttacker;          // +0x114
    void FinishAction();
};

void CCreatureAction_BanditKingGetHit::FinishAction()
{
    CCombatAction_GenericStrikeResponseBase::FinishAction();
    if (IS_THING_ALIVE(PAttacker) && (PAttacker.GetPItem()->m_flags6c & 8)) {
        CThing* attacker = PAttacker.GetPItem();
        CThingCreatureBase* creature = PCreature.GetPItem();
        if (IsHitFromBehind(creature, PAttacker.GetPItem())) {
            CCharString animation("REAR_ATTACK");
            CThingCreatureBase* self = PCreature.GetPItem();
            self->SetCurrentAction(CCreatureAction_BanditKingAttack(PCreature.GetPItem(), attacker, animation, 0, 1));
        }
        else {
            CCharString animation("STRIKE_MEDIUM_FRONT");
            CThingCreatureBase* self = PCreature.GetPItem();
            self->SetCurrentAction(CCreatureAction_BanditKingAttack(PCreature.GetPItem(), attacker, animation, 1, 0));
        }
    }
}