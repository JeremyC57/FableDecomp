// CAIStateGroup_MinionReactToTrespassing::Update() -- target the hero, then alternate between walking up to them and staring at them while the home is still being trespassed on and the hero lives
// Grade: DIFFER(1097v1098) -- one instruction off: the approach flag initialised at the top gives retail's register plan (ebp for the inlined lower_bound end, this spilled) but VC7.1 then hoists 'mov bl,1' to the top and reuses bl for the dead-bit test (test byte [eax+0x91],bl); initialising it later loses the register plan
// Layout: local model (the engine header flattens the base); PHero +0x14, LastFrameCheckedIsStillValid +0x1c; the creature's home pointer at retail +0x168.
struct CTCBase;
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91
    unsigned char _pad92[0x168 - 0x92];
    CBaseIntelligentPointer Home;               // +0x168
    CThing* GetHome() { return Home.GetPItem(); }
    // the fully inlined lower_bound form (no LowerBound call)
    template<class T> __forceinline bool GetInterfaceInline(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            CInterfaceMap::SEntry* last = m_interfaces.last;
            CInterfaceMap::SEntry* first = m_interfaces.first;
            long count = last - first;
            while (count > 0) {
                long half = count / 2;
                if (first[half].type < interfaceType) {
                    first += half + 1;
                    count -= half + 1;
                } else
                    count = half;
            }
            *ppOut = (T*)((first == last || interfaceType < first->type) ? last : first)->component;
            return true;
        }
        return false;
    }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& o) { SetPItem(o.GetPItem()); }
};
struct CIntelligentPointerPlayer : CBaseIntelligentPointer {       // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerPlayer(CThing* item) { SetPItem(item); }
};
struct COpinionDeed;
struct COpinionDeedFilter_IsStillHappening { COpinionDeedFilter_IsStillHappening() { } };
struct COpinionDeedFilter_TargetIs { CThing* Target; COpinionDeedFilter_TargetIs(CThing* p) : Target(p) { } };
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
struct CTCCreatureOpinionOfHero {               // interface 0x4f
    COpinionDeed* FindMostRecentDeed(long deedType, const CFilter_Bind2<COpinionDeedFilter_IsStillHappening, COpinionDeedFilter_TargetIs>& filter,
                                     long* index, bool includeReported);   // 0x0092E630
};
struct CPlayer {
    CThing* GetCharacter();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
extern long g_TrespassCheckInterval;            // 0x0138D2D4
long __fastcall GFGetCurrentFrame();            // 0x0049D870
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    void Terminate();                           // 0x008FCF40
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
};
struct CAIStateGroup_MinionReactToTrespassing : CAIStateGroupBase {
    CBaseIntelligentPointer PHero;              // +0x14
    long LastFrameCheckedIsStillValid;          // +0x1c
    __forceinline bool IsStillRunnable(bool initialCheck)
    {
        if (!initialCheck && GFGetCurrentFrame() - LastFrameCheckedIsStillValid < g_TrespassCheckInterval)
            return true;
        LastFrameCheckedIsStillValid = GFGetCurrentFrame();
        CThing* home = PBrain->PCreature->GetHome();
        if (home && !(home->m_flags91 & 1)) {
            CTCCreatureOpinionOfHero* opinion;
            if (PBrain->PCreature->GetInterfaceInline(&opinion, 0x4f)) {
                if (opinion->FindMostRecentDeed(10, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(home), 0, true)
                    || opinion->FindMostRecentDeed(11, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(home), 0, true)
                    || opinion->FindMostRecentDeed(2, COpinionDeedFilter_IsStillHappening() && COpinionDeedFilter_TargetIs(home), 0, true))
                    return true;
            }
        }
        return false;
    }
    void Update();
};

void CAIStateGroup_MinionReactToTrespassing::Update()
{
    bool approach = true;
    CThing* home = PBrain->PCreature->GetHome();
    if (home && !(home->m_flags91 & 1))
        PBrain->PCreature->GetHome();
    PHero.SetPItem(CIntelligentPointerPlayer(GetPPlayerManager()->GetMainPlayer()->GetCharacter()).GetPItem());
    long result;
    do {
        if (!IS_THING_ALIVE(PHero))
            break;
        if (approach) {
            do {
                if (!IS_THING_ALIVE(PHero)) {
                    Terminate();
                    return;
                }
                bool arrived;
                result = AIAction_MoveToThingProximity(2, PHero, 3.0f, &arrived, 3, true, false, true, true, 0.0f);
                if (!IsStillRunnable(false) || !IS_THING_ALIVE(PHero))
                    result = 1;
            } while (result == 4);
            approach = false;
        } else {
            do {
                if (!IS_THING_ALIVE(PHero)) {
                    Terminate();
                    return;
                }
                result = AIAction_LookAtThing(2, PHero, 0x13, 0.0027777778f);
                if (!IsStillRunnable(false) || !IS_THING_ALIVE(PHero))
                    result = 1;
            } while (result == 4);
            approach = true;
        }
    } while (result != 1);
    Terminate();
}
