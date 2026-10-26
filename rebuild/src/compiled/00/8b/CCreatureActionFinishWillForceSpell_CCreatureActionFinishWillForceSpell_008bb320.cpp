// CCreatureAction_FinishWillForceSpell::CCreatureAction_FinishWillForceSpell(CThingCreatureBase*, long) -- combat action 145 with the creature's FORCE_PUSH sound pair; the deliver animation follows the spell level (0..3)
// Leaf: ??0CCreatureAction_FinishWillForceSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BB320, 284 bytes + its 16-byte jump table (0x008BB43C, inside the catalog row),
//   `ret 8`; vtable 0x0127B87C.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("FORCE_PUSH" 0x01268358, "WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1..4"
//   0x0127B850 / 0x0127B828 / 0x0127B800 / 0x0127B7D8), the creature's virtual slot 89 (+0x164: the combat sound pair for a
//   name, CSoundPair by value through the hidden pointer), CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&)
//   0x00858030, ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation 0x008574D0.
// Shape: the sound pair is a by-value temporary in the base initializer (its name string an implicit conversion built
//   before the argument pushes); the vtable store is scheduled into the switch range test; a dense four-case switch
//   (jump table, `ja` to the shared epilogue) whose arms are implicit-conversion SetAnimationName calls -- each arm's
//   string temporary gets its own frame word (the dead argument slots and the two frame words), and VC duplicates the
//   epilogue into every arm.
class CThingCreatureBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CSoundPair { public:
    long FirstSound;
    long LastSound;
};
#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); virtual void v##n##4(); \
    virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); virtual void v##n##8(); virtual void v##n##9();
class CThingCreatureBase { public:
    V10(0) V10(1) V10(2) V10(3) V10(4) V10(5) V10(6) V10(7)                     // slots 0..79
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual CSoundPair GetCombatSoundPair(const CCharString& name);             // slot 89 (+0x164)
};
#undef V10

class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};
class CCreatureAction_FinishWillForceSpell : public CCombatActionBase { public:   // vtable 0x0127B87C
    virtual void v00();
    CCreatureAction_FinishWillForceSpell(CThingCreatureBase* owner, long level);
};

CCreatureAction_FinishWillForceSpell::CCreatureAction_FinishWillForceSpell(CThingCreatureBase* owner, long level)
    : CCombatActionBase(owner, 145, owner->GetCombatSoundPair("FORCE_PUSH"))
{
    switch (level) {
    case 0:
        SetCombatAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1");
        break;
    case 1:
        SetCombatAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_2");
        break;
    case 2:
        SetCombatAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_3");
        break;
    case 3:
        SetCombatAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_4");
        break;
    }
}