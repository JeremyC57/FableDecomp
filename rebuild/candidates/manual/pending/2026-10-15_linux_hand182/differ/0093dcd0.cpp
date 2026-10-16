// CAIStateGroup_MeleeCombatNearby2::IsEnemyCreatureInCombatPosition() -- in position when the combat wheel
// Grade: DIFFER(ours v retail) -- 219v219: x87 operand order only -- retail squares dx first (fld st(1) ; fmul st(2) twice), ours dy first (fld st(0) ; fmul st(1) / fld st(2) ; fmul st(3)) | tried: const-ref bound temporary (reads the frame copy), inline XYDelta helper returning by value (memory round trip), XYDistanceSquared member on the returned temporary (this file; everything else matches) | next: write the sum as dy * dy + dx * dx in XYDistanceSquared
// validates the creature's slot (then: whether it is in ring range), or, in ring range, when it has a valid melee
// sequence or stands within 1.5 units (squared 2.25, XY plane) of its preferred position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093DCD0, length 219 bytes.
// Callees: CThingCreatureBase::IsRegisteredCombatant @ 0x00662410, the brain world accessor @ 0x008FCFC0 (combat
// manager +0x4c), CCombatManager::Combat_CheckRingRangeToTarget @ 0x006E94D0 / ValidateSequencePosition
// @ 0x006E9700 / IsCreatureWithValidMeleeCombatSequence @ 0x006E9450 / GetPreferredPos (C3DVector by hidden
// pointer) @ 0x006EA770, CThing::GetPosition @ 0x004C73D0; the 2.25f constant @ 0x0125B2D4.
// Shape: the ring-range result is a bool local (bl, `mov bl,1` / `xor bl,bl`) through the inline
// GetPCombatManager() accessor (the landed sibling 0x0093DBD0 shape); for the distance the manager is a named
// local (ebx) fetched before the position, and the squared XY distance is an inline member called on the returned
// temporary (its `this` is the hidden-result pointer in eax; dx/dy stay on the FPU stack).
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
struct CCombatManager;
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
    CCombatManager* GetPCombatManager() { return CombatManager; }
    unsigned char _pad[0x10];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

struct C3DVector {
    float x, y, z;
    // squared XY distance (called on the returned temporary: `this` is the hidden-result pointer in eax)
    float XYDistanceSquared(const C3DVector& other) const
    {
        float dx = x - other.x;
        float dy = y - other.y;
        return dx * dx + dy * dy;
    }
};
struct CThingCreatureBase {
    bool IsRegisteredCombatant();               // 0x00662410
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CCombatManager {
    bool Combat_CheckRingRangeToTarget(CThingCreatureBase* creature);           // 0x006E94D0
    bool ValidateSequencePosition(CThingCreatureBase* creature);                // 0x006E9700
    bool IsCreatureWithValidMeleeCombatSequence(CThingCreatureBase* creature);  // 0x006E9450
    C3DVector GetPreferredPos(CThingCreatureBase* creature);                    // 0x006EA770
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase {
    unsigned char PEnemy[8];                    // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                                // +0x1c
    bool IsEnemyCreatureInCombatPosition();
};

bool CAIStateGroup_MeleeCombatNearby2::IsEnemyCreatureInCombatPosition()
{
    bool inRingRange;
    CThingCreatureBase* creature;
    if (((CThingCreatureBase*)PBrain->PCreature)->IsRegisteredCombatant()
        && (creature = (CThingCreatureBase*)PBrain->PCreature,
            GetPWorld()->GetPCombatManager()->Combat_CheckRingRangeToTarget(creature)))
        inRingRange = true;
    else
        inRingRange = false;
    creature = (CThingCreatureBase*)PBrain->PCreature;
    if (GetPWorld()->GetPCombatManager()->ValidateSequencePosition(creature))
        return inRingRange;
    if (inRingRange) {
        creature = (CThingCreatureBase*)PBrain->PCreature;
        if (GetPWorld()->GetPCombatManager()->IsCreatureWithValidMeleeCombatSequence(creature))
            return true;
        creature = (CThingCreatureBase*)PBrain->PCreature;
        CCombatManager* manager = GetPWorld()->CombatManager;
        const C3DVector& position = creature->GetPosition();
        if (manager->GetPreferredPos(creature).XYDistanceSquared(position) < 2.25f)
            return true;
    }
    return false;
}
