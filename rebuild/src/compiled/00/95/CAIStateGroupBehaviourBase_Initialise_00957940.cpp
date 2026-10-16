// CAIStateGroupBehaviourBase::Initialise(brain, creature, priorityValue, priority, name) -- base Initialise, then cache the creature's perception (0x34), opinion-of-hero (0x4F) and creature-mode (0x31) components and run the slot-14 hook
// Leaf: Initialise@CAIStateGroupBehaviourBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957940, length 216 bytes (`ret 14h`).
// Callees: CAIStateGroupBase::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// @ 0x008FCEC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (0x34, 0x4F) and non-const @ 0x0040F020
// (0x31), own virtual slot 14 (+0x38).
// Shape: no named locals, so every lookup key lives in the dead `name` parameter slot; PBrain is cached in ebx while the
// creature is re-read after each member store. Layout: Ego_r.pdb CAIStateGroupBehaviourBase = retail
// (PTCPerception +0x1c, PTCOpinion +0x20, PTCCreatureMode +0x24).
struct CTCBase;
struct CCharString;
struct CThingAICreature;

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCBase* GetTCConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
};

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CTCPerceiveThing;
struct CTCCreatureOpinionOfHero;
struct CTCCreatureModeManager;

struct CAIStateGroupBase
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void OnInitialised();                   // slot 14 (+0x38)
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long value, long priority,
                            const CCharString& name);    // 0x008FCEC0
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
};

struct CAIStateGroupBehaviourBase : CAIStateGroupBase
{
    unsigned char PTargetThing[8];                  // +0x14 CIntelligentPointer<const CThing>
    CTCPerceiveThing* PTCPerception;                // +0x1c
    CTCCreatureOpinionOfHero* PTCOpinion;           // +0x20
    CTCCreatureModeManager* PTCCreatureMode;        // +0x24
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long value, long priority,
                            const CCharString& name);
};

void CAIStateGroupBehaviourBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long value, long priority,
                                            const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, value, priority, name);
    CAIBrain* b = PBrain;
    if (b->Creature->HasTC(0x34))
        PTCPerception = (CTCPerceiveThing*)b->Creature->GetTCConst(0x34);
    if (b->Creature->HasTC(0x4f))
        PTCOpinion = (CTCCreatureOpinionOfHero*)b->Creature->GetTCConst(0x4f);
    if (b->Creature->HasTC(0x31))
        PTCCreatureMode = (CTCCreatureModeManager*)b->Creature->GetTC(0x31);
    OnInitialised();
}