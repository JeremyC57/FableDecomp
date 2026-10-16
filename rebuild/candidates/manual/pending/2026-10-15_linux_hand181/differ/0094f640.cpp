// CAIStateGroup_BalverineCornered::Update() -- with a live target: refresh the creature's volume tracker (0x75); inside a volume jump at the target, otherwise jump into the sky toward the stored landing position; terminate
// Grade: DIFFER(224v236) -- tail merging only: VC merges the two arms' `push eax ; mov ecx,esi ; call AIAction_CombatPerformActionAndWait ; lea ecx` where retail keeps one per arm and merges at the dtor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094F640, length 236 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (type 0x75 = word 3 bit 21), CTCVolumeContainmentTracker::Update @ 0x00785620, combat-action ctors @ 0x008CCC70
// (creature, target) and CCreatureAction_BalverineCorneredJumpIntoSky @ 0x008CCB80 (creature, target, const C3DVector&),
// CAIStateGroupBase::AIAction_CombatPerformActionAndWait @ 0x0097DA40, CCombatActionBase::~CCombatActionBase @ 0x0062E900,
// CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the tracker is deliberately unset when the interface is absent (retail reads a dead slot); `false` is pushed
// once before the arm split; each arm reads the creature into a named local first (kept in edi); both arms share the action dtor; the `result == 2` early terminate merges with the final one.
struct CThing
{
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCreatureActionBase
{
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

struct CCombatActionBase : CCreatureActionBase
{
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    ~CCombatActionBase();                           // 0x0062E900
};

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CWorld
{
    static long GetWorldFrame();                    // 0x0049D870
};

struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThingWithTCs
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20
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
};

struct CTCVolumeContainmentTracker                  // interface type 0x75
{
    unsigned char _pad00[0x0c];
    CBaseIntelligentPointer PVolume;                // +0x0c
    void Update();                                  // 0x00785620
};

struct CCombatAction_BalverineJumpAt : CCombatActionBase
{
    unsigned char _own[0x04];                       // retail action size 0x118
    CCombatAction_BalverineJumpAt(CThing* creature, CThing* target);   // 0x008CCC70
};

struct CCreatureAction_BalverineCorneredJumpIntoSky : CCombatActionBase
{
    unsigned char _own[0x04];                       // retail action size 0x118
    CCreatureAction_BalverineCorneredJumpIntoSky(CThing* creature, CThing* target, const C3DVector& land);   // 0x008CCB80
};

struct CAIStateGroup_BalverineCornered
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14
    unsigned char _pad1c[0x28 - 0x1c];
    C3DVector LandPos;                              // +0x28
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
    void Terminate();                               // 0x008FCF40
    void Update();
};

void CAIStateGroup_BalverineCornered::Update()
{
    if (IS_THING_ALIVE(PTargetThing))
    {
        CThingWithTCs* creature = (CThingWithTCs*)PBrain->Creature;
        CTCVolumeContainmentTracker* tracker;
        if (creature->HasTC(0x75))
            tracker = (CTCVolumeContainmentTracker*)creature->GetTC(0x75);
        tracker->Update();
        long result;
        if (tracker->PVolume.GetPItem())
        {
            CThing* self = PBrain->Creature;
            result = AIAction_CombatPerformActionAndWait(
                &CCombatAction_BalverineJumpAt(self, PTargetThing.GetPItem()), false);
        }
        else
        {
            CThing* self = PBrain->Creature;
            result = AIAction_CombatPerformActionAndWait(
                &CCreatureAction_BalverineCorneredJumpIntoSky(self, PTargetThing.GetPItem(), LandPos), false);
        }
        if (result == 2)
        {
            Terminate();
            return;
        }
    }
    Terminate();
}
