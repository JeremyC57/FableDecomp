// CAIStateGroup_GoToBed::IsUnreportedCrimeKnown() -- does this creature know of a hero crime it should run to report, or an ongoing crime against its home?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092D060, length 388 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0 (the creature is reached through a const
//          getter), CTCVillageMember::GetPVillage() const @ 0x007EE540 (interface 0x23; manifest "IS_THING_ALIVE<...>" is
//          propagated, landed callers 0x00833710 / 0x007B6A80 name it), CTCCreatureOpinionOfHero::FindMostRecentDeed<
//          COpinionDeedFilter_IsCrimeToRunAndFindAGuard> @ 0x006610C7 and FindMostRecentDeed<CFilter_Bind2<
//          COpinionDeedFilter_IsStillHappening, COpinionDeedFilter_TargetIs> > @ 0x0092E630, CBaseIntelligentPointer::GetPItem
//          @ 0x00A01B50.
// Shape: HasTC returns the masked bitset word (`test byte ptr [eax+24h],8` for 0x23, `test ch,ch ; jns` for 0x4F);
//          the components come from the ternary GetTC pick (0x23 lands in ecx as `this`, the opinion in esi); the
//          tests are nested so every miss shares the one `xor al,al` epilogue;
//          the brain's creature is re-read for every use; the home is fetched again for each TargetIs filter
//          (filter objects: the empty filters in frame byte +3, TargetIs at +4, the bind pair at +8). Deed types
//          10, 11 and 2 are tried in that order.
// Layout: creature Home intelligent pointer at retail +0x168 (as the landed 0x009580E0); bitset +0x20, interface map
//          +0x44, dead flag +0x91. No engine header exists for CAIStateGroup_GoToBed.
struct CTCBase;
struct CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CThingPhysical;
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x168 - 0x92];
    CBaseIntelligentPointer Home;               // +0x168 (CThingAICreature)
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

struct CTCVillage;
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540
};

struct COpinionDeed;
struct COpinionDeedFilter_IsCrimeToRunAndFindAGuard { COpinionDeedFilter_IsCrimeToRunAndFindAGuard() {} };
struct COpinionDeedFilter_IsStillHappening { COpinionDeedFilter_IsStillHappening() {} };
struct COpinionDeedFilter_TargetIs { CThing* Target; COpinionDeedFilter_TargetIs(CThing* p) : Target(p) {} };
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {}
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

struct CTCCreatureOpinionOfHero {               // interface 0x4f
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* index, bool includeReported);   // 0x006610C7
    COpinionDeed* FindMostRecentDeed(long deedType, const CFilter_Bind2<COpinionDeedFilter_IsStillHappening, COpinionDeedFilter_TargetIs>& filter,
                                     long* index, bool includeReported);   // 0x0092E630
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
    const CThing* GetPCreature() const { return PCreature; }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CBaseIntelligentPointer PBed;               // +0x14
    bool IsBedSleepPointMarker;                 // +0x1c
    bool IsUnreportedCrimeKnown();
};

bool CAIStateGroup_GoToBed::IsUnreportedCrimeKnown()
{
    if (PBrain->GetPCreature()->HasTC(0x23)
        && PBrain->GetPCreature()->GetTC<CTCVillageMember>(0x23)->GetPVillage()
        && PBrain->GetPCreature()->HasTC(0x4f)) {
        CTCCreatureOpinionOfHero* opinion = PBrain->GetPCreature()->GetTC<CTCCreatureOpinionOfHero>(0x4f);
        if (opinion->FindMostRecentDeed(COpinionDeedFilter_IsCrimeToRunAndFindAGuard(), 0, true))
            return true;
        CThing* home = PBrain->GetPCreature()->Home.GetPItem();
        if (home && !(home->m_flags91 & 1)) {
            if (opinion->FindMostRecentDeed(10, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(PBrain->GetPCreature()->Home.GetPItem()), 0, true)
                || opinion->FindMostRecentDeed(11, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(PBrain->GetPCreature()->Home.GetPItem()), 0, true)
                || opinion->FindMostRecentDeed(2, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(PBrain->GetPCreature()->Home.GetPItem()), 0, true))
                return true;
        }
    }
    return false;
}
