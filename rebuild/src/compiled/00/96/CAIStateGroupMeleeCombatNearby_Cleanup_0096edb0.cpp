// CAIStateGroup_MeleeCombatNearby::Cleanup() -- settle the creature's combat-manager state (attack query, then
// deregistration while still registered) and drop creature mode 0x19 when it is in combat mode.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096EDB0, length 222 bytes (+ int3 padding). Vtable slot 9.
// Callees: CThingCreatureBase::IsRegisteredCombatant @ 0x00662410 (three times), the brain world accessor
// @ 0x008FCFC0 (combat manager +0x4c), CCombatManager::IsCreatureAttacking @ 0x006E92C0 (twice; the second call's
// result unused; the same function, not a folded twin), CCombatManager::DeregisterCombatant @ 0x006ED9C0,
// CThingCreatureBase::IsInCombatMode @ 0x006621A0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface,
// 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::RemoveMode(ulong) @ 0x0073ABA0.
// Shape: every manager call reads the creature into a named local before a named world (edi across the call); the
// first query goes through the inline GetPCombatManager() accessor (`mov eax,[eax+0x4c] ; push edi ; mov ecx,eax`),
// the plain member gives `mov ecx,[eax+0x4c]`;
// the mode-manager lookup used unconditionally with the RemoveMode call duplicated on both paths.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual bool IsStillRunnable(bool checkEnemy);   // slot 14 (+0x38)
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
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

class CMessageEventFilter_HasExtraData { public:
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

class CMessageEvent;
class CMessageEventManager;
class CCombatManager;
class CWorld { public:
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
    CCombatManager* GetPCombatManager() const { return CombatManager; }
    unsigned char _pad[0x10];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

class CThingCreatureBase { public:
    bool IsRegisteredCombatant();               // 0x00662410 (manifest label C3DAnimationBlendState::AddCombinationBlend propagated)
    bool IsInCombatMode();                      // 0x006621A0 (manifest label AreDefaultValsApplied propagated)
};

enum EAIActionReturn { AI_ACTION_RETURN_NONE, AI_ACTION_RETURN_1, AI_ACTION_RETURN_SUCCEEDED, AI_ACTION_RETURN_3, AI_ACTION_RETURN_4 };

class CCombatManager { public:
    void SetAttackerAsReadyToAttack(CThingCreatureBase* attacker, bool ready);   // 0x006EA450
    bool IsAttackerReadyToAttack(CThingCreatureBase* attacker);                  // 0x006E93C0
    bool IsCreatureAttacking(CThingCreatureBase* creature);                      // 0x006E92C0
    void DeregisterCombatant(CThingCreatureBase* creature);                      // 0x006ED9C0
};

class CTCCreatureModeManager { public:                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CAIStateGroup_MeleeCombatNearby : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                                // +0x1c (engine header)
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    EAIActionReturn AIAction_PumpCombatActions();   // 0x0097D980
    bool IsEnemyCreatureNearCombatPosition();   // 0x0096E830
    void AttackEnemy();
    virtual void Cleanup();
};

void CAIStateGroup_MeleeCombatNearby::Cleanup()
{
    CThingCreatureBase* creature;
    CWorld* world;
    if (!((CThingCreatureBase*)PBrain->PCreature)->IsRegisteredCombatant()
        || (creature = (CThingCreatureBase*)PBrain->PCreature, world = GetPWorld(),
            world->GetPCombatManager()->IsCreatureAttacking(creature))) {
        if (!((CThingCreatureBase*)PBrain->PCreature)->IsRegisteredCombatant()) {
            creature = (CThingCreatureBase*)PBrain->PCreature;
            world = GetPWorld();
            world->CombatManager->IsCreatureAttacking(creature);   // retail calls 0x006E92C0 again (no ICF in this build: duplicate bodies stay separate)
        }
    }
    if (((CThingCreatureBase*)PBrain->PCreature)->IsRegisteredCombatant()) {
        creature = (CThingCreatureBase*)PBrain->PCreature;
        world = GetPWorld();
        world->CombatManager->DeregisterCombatant(creature);
    }
    if (((CThingCreatureBase*)PBrain->PCreature)->IsInCombatMode()) {
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x19);
    }
}