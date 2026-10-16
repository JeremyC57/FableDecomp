// CAIStateGroup_MeleeCombatNearbyBase::ShouldGoStraightIntoStrafe() -- in combat mode, with a live enemy and the
// group running: strafe straight away when the creature is nearer the enemy (XY plane) than its preferred combat
// position is.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D180, length 236 bytes (+ int3 padding).
// Callees: CThingCreatureBase::IsInCombatMode @ 0x006621A0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy
// +0x14), IsStopping @ 0x008FD010, CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::
// GetPreferredPos (C3DVector by hidden pointer) @ 0x006EA770, CThing::GetPosition @ 0x004C73D0.
// Shape: the preferred position is a named 12-byte local built in place (its arguments pushed before the manager
// call); two XY deltas from the enemy position: the first a 12-byte local whose z is never written (retail keeps
// the dead slot: a C2DVector gives a 4-byte smaller frame) stored across the calls, the second on the
// FPU), compared by squared length (`fcompp ; test ah,41h`).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_HasExtraData {
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
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

struct CMessageEvent;
struct CMessageEventManager;
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

struct C3DVector { float x, y, z; };
struct C2DVector {
    float x, y;
    float LengthSquared() const { return x * x + y * y; }
};
// a 12-byte delta whose z is never written (retail's frame keeps the unused slot)
struct CXYDelta {
    float x, y, z;
    float LengthSquaredXY() const { return x * x + y * y; }
};

struct CCombatManager;
struct CThingCreatureBase {
    bool IsInCombatMode();                      // 0x006621A0
    CCombatManager* GetCombatManager();         // 0x00661ED0
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CCombatManager {
    C3DVector GetPreferredPos(CThingCreatureBase* creature);   // 0x006EA770
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                                // +0x1c
    bool IsStopping();                          // 0x008FD010
    CThingCreatureBase* GetPEnemy() const { return (CThingCreatureBase*)PEnemy.GetPItem(); }
    bool ShouldGoStraightIntoStrafe();
};

bool CAIStateGroup_MeleeCombatNearbyBase::ShouldGoStraightIntoStrafe()
{
    if (((CThingCreatureBase*)PBrain->PCreature)->IsInCombatMode() && IS_THING_ALIVE(PEnemy) && !IsStopping()) {
        CThingCreatureBase* self = (CThingCreatureBase*)PBrain->PCreature;
        C3DVector preferred = self->GetCombatManager()->GetPreferredPos(self);
        CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
        const C3DVector& enemyPos = GetPEnemy()->GetPosition();
        const C3DVector& creaturePos = creature->GetPosition();
        CXYDelta toCreature;
        toCreature.x = creaturePos.x - enemyPos.x;
        toCreature.y = creaturePos.y - enemyPos.y;
        const C3DVector& enemyPos2 = GetPEnemy()->GetPosition();
        C2DVector toPreferred = { preferred.x - enemyPos2.x, preferred.y - enemyPos2.y };
        if (toCreature.LengthSquaredXY() < toPreferred.LengthSquared())
            return true;
    }
    return false;
}