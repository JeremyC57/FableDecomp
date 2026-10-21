// CAIStateGroup_JackDragon::Update() -- target the main player's character, then until the enemy dies or the group stops: advance the boss phase when ready and run the current dragon state's handler; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00952E80, length 384 bytes including the 24-byte jump table at 0x00952FE4.
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
//          CPlayer::GetPCharacter @ 0x00487DC0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary vtable
//          0x0123E7D8), CAIStateGroupBase::IsStopping @ 0x008FD010, the inlined STLport lower_bound of the interface map
//          (0xad = CTCBoss, bit 13 of +0x34), CTCBoss::IsReadyForPhaseChange @ 0x007AEE90 / IncrementBossPhase @ 0x007AED20,
//          the state handlers IntoGround @ 0x00951070, UpdateGround @ 0x00952890, IntoHover @ 0x00951D60, UpdateHover
//          @ 0x00951FB0, UpdateStrafe @ 0x00950CF0, IntoGroundSwoop @ 0x00951430, Terminate @ 0x008FCF40.
// Shape: `PEnemy = CIntelligentPointer<CThingCreatureBase>(character)` through a frame temporary; the boss pointer is
//          declared outside the loop and only written on a hit, so its never-written home (the temporary's slot) is
//          loaded into esi once before the loop; the six-state switch is a jump table whose default loops again.
// Layout: the generated engine/CAIStateGroup_JackDragon.h is packed (#pragma pack(1)) and misplaces every member after
//          +0x64, so the class is modelled locally like the landed JackDragon rows (PEnemy +0x14, CurrentState +0x5c).
struct CTCBase;
struct CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)        // STLport lower_bound over the 8-byte entries (inlined in this TU)
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
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CPlayer {
    CThing* GetPCharacter();                    // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CTCBoss {                                // interface 0xad
    bool IsReadyForPhaseChange();               // 0x007AEE90
    void IncrementBossPhase();                  // 0x007AED20
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
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
    CThing* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    CBaseIntelligentPointer PLandedMarker;      // +0x1c
    CBaseIntelligentPointer PHoverMarker1;      // +0x24
    CBaseIntelligentPointer PHoverMarker2;      // +0x2c
    CBaseIntelligentPointer PStrafeMarker1;     // +0x34
    CBaseIntelligentPointer PStrafeMarker2;     // +0x3c
    CBaseIntelligentPointer PInvisibleBodgeCreature;  // +0x44
    long FrameOfLastSlam;                       // +0x4c
    long FrameWhenLastInRange;                  // +0x50
    long FrameWhenLanded;                       // +0x54
    long FramesInSnapRange;                     // +0x58
    long CurrentState;                          // +0x5c EDragonState
    void IntoGround();                          // 0x00951070
    void UpdateGround();                        // 0x00952890
    void IntoHover();                           // 0x00951D60
    void UpdateHover();                         // 0x00951FB0
    void UpdateStrafe();                        // 0x00950CF0
    void IntoGroundSwoop();                     // 0x00951430
    virtual void Update();
};

void CAIStateGroup_JackDragon::Update()
{
    PEnemy = CIntelligentPointerCreature(GetPPlayerManager()->GetMainPlayer()->GetPCharacter());
    CTCBoss* boss;
    while (IS_THING_ALIVE(PEnemy) && !IsStopping()) {
        GetCreature()->GetInterface(&boss, 0xad);
        if (boss->IsReadyForPhaseChange())
            boss->IncrementBossPhase();
        switch (CurrentState) {
        case 0: IntoGround(); break;
        case 1: UpdateGround(); break;
        case 2: IntoHover(); break;
        case 3: UpdateHover(); break;
        case 4: UpdateStrafe(); break;
        case 5: IntoGroundSwoop(); break;
        }
    }
    Terminate();
}
