// CAIStateGroup_MinionGuardHeardYellForHelp::Update() -- find the latest yell-for-help message (type 0xb) from a living originator the guard can see or hear, consume it, look at the yelling creature, run to within 3 units of it and, when the hero is alive, declare him a criminal; terminate when done or when the yeller dies
// Grade: DIFFER(500v500) -- the originator's creature-type test is `xor eax,0xf ; test al,0xf` in retail (the out-of-line copy 0x00910CC0 has `not eax`); ours gives `and eax,0xf ; cmp al,0xf` (mask form or this bitfield) or `not eax` (complement/xor forms); and retail loads the brain into eax before the creature's slot-92 call (ours ecx, ecx)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00910DF0, length 500 bytes (+ int3 padding), plain `ret` (virtual), `and esp,-8`
// frame for the 8-aligned action object.
// Callees: the state group's thing-world accessor @ 0x008FCFA0 and world accessor @ 0x008FCFC0 (message manager at +0x60),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<IsOfType, IsOriginatorAlive, CanBeSeenOrHeard>> @ 0x00910CF0,
// CMessageEventManager::RemoveMessage @ 0x00493B5B, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 /
// SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (CIntelligentPointer<CThingCreatureBase> vtable 0x0125AAEC,
// CIntelligentPointer<const CThing> vtable 0x01237F1C), the creature's virtual slot 92 (+0x170, a float),
// AIAction_LookAtThing @ 0x0097D6F0, AIAction_MoveToThingProximity @ 0x0097E380, the player-manager accessor @ 0x008FCF90,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0,
// CCreatureAction_DeclarePlayerACriminal ctor @ 0x00910C30 / dtor @ 0x00910B20, AIAction_PerformActionAndWait @ 0x0097DB70,
// Terminate @ 0x008FCF40.
// Data: 1/360 (0.0027777778f) @ 0x01238E00; 3.0f immediate.
// Shape: the message filter is the landed 0x00939D70 expression chain (`IsOfType(0xb) && IsOriginatorAlive() &&
// CanBeSeenOrHeard(creature)`, Bind objects of filter pointers); the originator-creature accessor (out of line at 0x00910CC0)
// is inline here; the intelligent pointers convert through a template copy constructor (base ctor before GetPItem).
// Layout: CMessageEvent Type +8, POriginator +0x48 (Ego_r.pdb); retail CThing ThingTypeFlags +0x6c (creature = low nibble 0xf).
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
struct CThing {
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF10(7) VF10(8) VF(90) VF(91)
    unsigned char _pad4[0x6c - 4];
    unsigned long ThingType : 4;                // +0x6c ThingTypeFlags (creature = 0xf)
    unsigned long ThingTypeRest : 28;
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsCreature() const { return ThingType == 0xf; }
};
struct CThingCreatureBase : CThing { };
struct CThingPlayerCreature : CThingCreatureBase { };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem(other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }

struct CMessageEvent {
    unsigned char _pad0[0x48];
    CIntelligentPointer<CThing> POriginator;    // +0x48
    CThingCreatureBase* GetPOriginatorCreature() const
    {
        CThingCreatureBase* creature = 0;
        if (POriginator.GetPItem() != 0 && POriginator.GetPItem()->IsCreature())
            creature = (CThingCreatureBase*)POriginator.GetPItem();
        return creature;
    }
};

struct CThingAICreature : CThingCreatureBase {
    virtual float GetTurnSpeed();               // slot 92 (+0x170)
};

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
struct CMessageEventFilter_IsOriginatorAlive {
    CMessageEventFilter_IsOriginatorAlive() { }
};
struct CMessageEventFilter_CanBeSeenOrHeard {
    CThingAICreature* m_pCreature;
    CMessageEventFilter_CanBeSeenOrHeard(CThingAICreature* creature) : m_pCreature(creature) { }
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatorAlive, CMessageEventFilter_CanBeSeenOrHeard> CYellFilter;

struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CYellFilter& filter);   // 0x00910CF0
    void RemoveMessage(const CMessageEvent* message);                   // 0x00493B5B
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};
struct CPlayer {
    CThingPlayerCreature* GetHeroCreature();    // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};

struct CCreatureActionBase {
    void* __vftable;
    long _pad04;
    __int64 _align08;                           // 8-aligned (the `and esp,-8` frame)
    unsigned char _pad10[0xb0 - 0x10];
};
struct CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase {
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* creature, CThingPlayerCreature* hero);   // 0x00910C30
    ~CCreatureAction_DeclarePlayerACriminal();  // 0x00910B20
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<const CThing> thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_MinionGuardHeardYellForHelp : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_MinionGuardHeardYellForHelp::Update()
{
    CMessageEvent* message = GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0xb) && CMessageEventFilter_IsOriginatorAlive() &&
        CMessageEventFilter_CanBeSeenOrHeard(GetCreature()));
    if (message) {
        CIntelligentPointer<CThingCreatureBase> yeller(message->GetPOriginatorCreature());
        GetPWorld()->GetPMessageManager()->RemoveMessage(message);
        if (AIAction_LookAtThing(1, yeller, 0x23, GetCreature()->GetTurnSpeed() * 0.0027777778f) != 2) {
            Terminate();
            return;
        }
        bool arrived = false;
        for (;;) {
            if (!IsThingAlive(yeller)) {
                Terminate();
                return;
            }
            long result = AIAction_MoveToThingProximity(2, yeller, 3.0f, &arrived, 5, true, false, true, true, 0.0f);
            if (result == 4)
                continue;
            if (result != 2) {
                Terminate();
                return;
            }
            break;
        }
        CThingPlayerCreature* hero = GetPPlayerManager()->GetMainPlayer()->GetHeroCreature();
        if (hero != 0 && !(hero->m_flags91 & 1))
            AIAction_PerformActionAndWait(&CCreatureAction_DeclarePlayerACriminal(GetCreature(), hero), false);
    }
    Terminate();
}
