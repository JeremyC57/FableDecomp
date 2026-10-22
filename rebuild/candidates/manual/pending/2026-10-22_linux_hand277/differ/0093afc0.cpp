// Grade: DIFFER(522v522) -- only the seed store is scheduled before the range load (retail loads g_SitDelayRange, compares, then stores the GFROR13 result)
// CAIStateGroup_BanditSitRoundCampfire::OnReplace() -- free the campfire seat, and if the bandit is sitting, schedule the next sit, ask the brain to stand up and drop the sitting mode; re-draw the weapon if it was carried
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
long __fastcall GFROR13(unsigned long value);    // 0x00497890

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long type) const
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
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    bool IsFreeToPerformAction(const CCreatureActionBase& action);   // 0x00662210
    void UnsheatheDefaultWeapon();              // 0x00664A50
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);
    CCreatureActionBase* m_pObject;
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
    void Reset(CCreatureActionBase* object);    // 0x00668D90
    __forceinline ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct C3DVector { float X, Y, Z; };
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a();
    virtual CCreatureActionBase* Clone() const; // slot 11 (+0x2c)
};
struct CNavigationInput {
    C3DVector Destination;
    float Proximity;
    long MovementType;
    float Speed;
};
struct CBrainOutput {
    unsigned long                       Types;            // +0x00
    long                                LookFlags;        // +0x04
    CNavigationInput                    NavigationInput;  // +0x08
    CCountedPointer_CCreatureActionBase PAction;          // +0x20
    C3DVector                           MoveToPos;        // +0x28
    C3DVector                           LookAtPos;        // +0x34
    CBaseIntelligentPointer             LookAtThing;      // +0x40
    CBrainOutput();                             // 0x00835620
    ~CBrainOutput() { }
    void SetAction(const CCreatureActionBase& action)
    {
        Types |= 8;
        PAction.Reset(action.Clone());
    }
};
struct CCharString { void* PStringData; };
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                       // 0x0062E900
    void SetAnimation(const CCharString& name); // 0x008574D0
};
struct CCreatureAction_StandUp : CCombatActionBase {   // vtable 0x01276C2C
    virtual void v00();
    long m_114;
    bool m_118;
    CCreatureAction_StandUp(CThingCreatureBase* owner) : CCombatActionBase(owner, 50, CSoundPair()), m_114(0), m_118(false) { }
};
extern CCharString g_SitStandUpAnim;            // 0x013BB708
extern unsigned long g_SitDelayBase;            // 0x013BB70C
extern unsigned long g_SitDelayRange;           // 0x013BB710

struct CCreatureMode { virtual void v00(); virtual long GetType() const; };
struct CTCCreatureModeManager {
    CCreatureMode* GetCurrentMode();            // 0x0073A6E0
    void RemoveMode(long type);                 // 0x0073ABA0
};
struct CTCUserActionPointBase {
    void FreeActionPoint(long index);           // 0x007FEF40
};
struct CWorld { static unsigned long GetWorldFrame(); };   // 0x0049D870
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
    void SetOutput(const CBrainOutput& output); // 0x0088B8F0
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld() const;                  // 0x008FCFA0
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    unsigned long Random(const unsigned long& range) const
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};
struct CAIStateGroup_BanditSitRoundCampfire : CAIStateGroupBase {
    CBaseIntelligentPointer PCampfire;          // +0x14
    long SeatPosIndex;                          // +0x1c
    bool GotSeatPos;                            // +0x20
    bool Sitting;                               // +0x21
    unsigned long FrameToNextSitOnSeat;         // +0x24
    unsigned long FrameToNextCheckIsRunnable;   // +0x28
    bool WasCarryingWeapon;                     // +0x2c
    void OnReplace();
};

void CAIStateGroup_BanditSitRoundCampfire::OnReplace()
{
    if (GotSeatPos && PCampfire.GetPItem() && !(PCampfire.GetPItem()->m_flags91 & 1)) {
        CTCUserActionPointBase* point;
        CThing* campfire = PCampfire.GetPItem();
        if (campfire->HasTC(0x2d))
            point = (CTCUserActionPointBase*)campfire->GetTC(0x2d);
        point->FreeActionPoint(SeatPosIndex);
    }
    CTCCreatureModeManager* modes;
    CThingCreatureBase* creature = GetCreature();
    if (creature->HasTC(0x31))
        modes = (CTCCreatureModeManager*)creature->GetTC(0x31);
    if (Sitting || modes->GetCurrentMode()->GetType() == 10) {
        GetPWorld();
        unsigned long delay = Random(g_SitDelayRange);
        FrameToNextSitOnSeat = CWorld::GetWorldFrame() + g_SitDelayBase + delay;
        CBrainOutput output;
        CCreatureAction_StandUp action(GetCreature());
        action.SetAnimation(g_SitStandUpAnim);
        if (GetCreature()->IsFreeToPerformAction(action)) {
            output.SetAction(action);
            PBrain->SetOutput(output);
        }
        modes->RemoveMode(10);
        Sitting = false;
    }
    if (WasCarryingWeapon)
        GetCreature()->UnsheatheDefaultWeapon();
}
