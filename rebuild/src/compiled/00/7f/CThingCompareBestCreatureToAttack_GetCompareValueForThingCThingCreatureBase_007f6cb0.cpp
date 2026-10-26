// CThingCompare_BestCreatureToAttack::GetCompareValueForThing<CThingCreatureBase>(CThingCreatureBase* thing) const -- attack-target score: 1 / (sqrt(2D distance) + 1), weighted for the player, non-combatants, the targeter's last attacker and the current best target
// Leaf: GetCompareValueForThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F6CB0, length 179 bytes, `ret 4`, float in st0. Byte-identical to
//   the sibling instantiation at 0x007F6BF0. Both catalog rows say <CThingAICreature>; the two callers are the
//   operator()<T> pair at 0x007F6F20/0x007F6FD0, so one row is the <CThingCreatureBase> instantiation (this file
//   takes that one; which address is which T is not provable from the bytes).
// Callees: CThing::GetPosition @ 0x004C73D0, CThingCreatureBase::IsPlayer (vtable slot 77, +0x134),
//   CThingCreatureBase::IsInMeleeAsAttacked @ 0x00662430 (asks the world's combat manager; manifest label
//   "C3DAnimationBlendState::AddCombinationBlend" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Float pool: 0x0122DED8 = 1.0f (both the `fadd` and the `fdivr`).
// Layout: CThingCompare_BestCreatureToAttack (Ego_r.pdb, 24 bytes): Targeter +0x00, PCurrentBestTargetForAttack
//   +0x04, PreferPlayerWeight +0x08, PreferNonCombatantsWeight +0x0c, PreferLastAttackerWeight +0x10,
//   PreferCurrentBestEnemyWeight +0x14. Retail CThing: ThingTypeFlags +0x6c (THING_TYPE bit 3 = AI creature);
//   retail CThingCreatureBase PLastAttackedByCreature +0xf4 (PDB +0x110; the engine header carries PDB offsets,
//   so the thing classes are modelled locally).
// Shape: the creature cast is the THING_TYPE test (`(flags & mask) == mask` narrowed to `test al,8`) into a
//   zero-initialised local; the 2D delta is a C2DVector local whose length VC keeps on the x87 stack (the dead
//   components are popped with `fstp st(2) ; fstp st(0)`), the square terms come Y first; the score is a named
//   float local spilled into the dead argument slot across the virtual call.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);

class CBaseObject { public:
    virtual void v00();
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* GetPItem() const;                              // 0x00A01B50
    CBaseObject* m_pItem;
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetTypedItem() const { return (T*)GetPItem(); }
};

class C3DVector { public: float X, Y, Z; };

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLength() const { return (float)sqrt(Y * Y + X * X); }
};

class CThing : public CBaseObject { public:
    unsigned char _pad04[0x6c - 0x04];
    unsigned long ThingTypeFlags;                               // +0x6c
    const C3DVector& GetPosition() const;                       // 0x004C73D0
};

template<class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();

class CThingCreatureBase : public CThing { public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6)
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76();
    virtual bool IsPlayer() const;                              // slot 77 (+0x134)
    unsigned char _pad70[0xf4 - 0x70];
    CIntelligentPointer<CThingCreatureBase> PLastAttackedByCreature;   // +0xf4
    CThingCreatureBase* GetLastAttackedByCreature() const { return PLastAttackedByCreature.GetTypedItem(); }
public:
    bool IsInMeleeAsAttacked() const;   // 0x00662430
};

class CThingAICreature : public CThingCreatureBase { public:
    enum { ThingTypeMask = 8 };
};

class CThingCompare_BestCreatureToAttack { public:
    CThingCreatureBase* Targeter;                               // +0x00
    CThing* PCurrentBestTargetForAttack;                        // +0x04
    float PreferPlayerWeight;                                   // +0x08
    float PreferNonCombatantsWeight;                            // +0x0c
    float PreferLastAttackerWeight;                             // +0x10
    float PreferCurrentBestEnemyWeight;                         // +0x14
    template<class T> float GetCompareValueForThing(T* thing) const;
};

template<class T> float CThingCompare_BestCreatureToAttack::GetCompareValueForThing(T* thing) const
{
    CThingAICreature* creature = 0;
    if (IsThingOfType<CThingAICreature>(thing))
        creature = (CThingAICreature*)thing;
    const C3DVector& targeterPos = Targeter->GetPosition();
    const C3DVector& thingPos = thing->GetPosition();
    C2DVector delta(thingPos.X - targeterPos.X, thingPos.Y - targeterPos.Y);
    float distance = delta.GetLength();
    float value = 1.0f / ((float)sqrt(distance) + 1.0f);
    if (thing->IsPlayer())
        value *= PreferPlayerWeight;
    if (creature) {
        if (!creature->IsInMeleeAsAttacked())
            value *= PreferNonCombatantsWeight;
        if (Targeter->GetLastAttackedByCreature() == creature)
            value *= PreferLastAttackerWeight;
    }
    if (PCurrentBestTargetForAttack && PCurrentBestTargetForAttack == thing)
        value *= PreferCurrentBestEnemyWeight;
    return value;
}

template float CThingCompare_BestCreatureToAttack::GetCompareValueForThing<CThingCreatureBase>(CThingCreatureBase* thing) const;