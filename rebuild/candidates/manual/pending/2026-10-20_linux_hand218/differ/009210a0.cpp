// CAIStateGroup_BeetleToggleHover::IsRunnable() -- toggle a beetle between hovering and landing: land (when hovering) once the visible enemy is over 10 units away in XY, take off when it is within 5
// Grade: DIFFER(ours v retail) -- register assignment only: ours keeps this in ebx and the creature in edi (retail this in edi, the creature in an in-branch push ebx)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009210A0, 290 bytes (virtual, `ret`). Ego_r.pdb: ?IsRunnable@CAIStateGroup_BeetleToggleHover@@MAE_NXZ.
// Callees: CVectorMap::LowerBound 0x0040F020 (inlined HasTC + entry-returning FindEntry, types 8 and 0x31), the enemy
// source's virtual slot 29 (+0x74), CanCreatureSeeCreature 0x008645E0 (`__fastcall` viewer, target, false),
// CThing::GetPosition 0x004C73D0 (twice; the enemy first), CTCCreatureModeManager::GetCurrentMode 0x0073A6E0, the
// mode's virtual slot 1 (+4, type; 0x1A = hover); float pool 1e10f (immediate), 100.0f 0x01230A08, 25.0f 0x0123003C.
// Shape: the landed AI state-group models (0x00950A30 non-const LowerBound, 0x00937F70 enemy lookup); the XY squared
// distance from two named float deltas (VC emits the Y term first), IsLanding stored from the mode test.
// Layout: Ego_r.pdb CAIStateGroup_BeetleToggleHover IsLanding +0x14.
// Leaf: IsRunnable@CAIStateGroup_BeetleToggleHover
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    const struct C3DVector& GetPosition() const;   // 0x004C73D0
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


struct C3DVector { float X, Y, Z; };

struct CTCEnemySource {                         // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};

struct CCreatureModeBase {
    virtual void v00();
    virtual long GetType() const;               // slot 1 (+4)
};
struct CTCCreatureModeManager {                 // the interface-0x31 component
    CCreatureModeBase* GetCurrentMode();        // 0x0073A6E0
};

bool __fastcall CanCreatureSeeCreature(const CThing* viewer, const CThing* target, bool ignoreLight);   // 0x008645E0

static inline float XYDistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}

struct CAIStateGroup_BeetleToggleHover : CAIStateGroupBase {
    bool IsLanding;                             // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_BeetleToggleHover::IsRunnable()
{
    float distanceSquared = 1e10f;
    if (PBrain->PCreature->HasTC(8)) {
        CThing* enemy = ((CTCEnemySource*)PBrain->PCreature->FindEntry(8)->component)->GetEnemy();
        if (enemy != 0 && !(enemy->m_flags91 & 1) && CanCreatureSeeCreature(PBrain->PCreature, enemy, false)) {
            CThing* creature = PBrain->PCreature;
            distanceSquared = XYDistanceSquared(creature->GetPosition(), enemy->GetPosition());
        }
    }
    if (PBrain->PCreature->HasTC(0x31)) {
        IsLanding = ((CTCCreatureModeManager*)PBrain->PCreature->FindEntry(0x31)->component)->GetCurrentMode()->GetType() == 0x1a;
        if (IsLanding) {
            if (distanceSquared > 100.0f)
                return true;
        }
        else if (distanceSquared < 25.0f)
            return true;
    }
    return false;
}
