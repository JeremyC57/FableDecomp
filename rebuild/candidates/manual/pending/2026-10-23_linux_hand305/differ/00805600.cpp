// Grade: DIFFER(641v641) -- NEAR-MISS, one register: before the Z-targetable test retail loads ParentThing into edx (`mov edx,[esi+4] ; test dword ptr [edx+30h],100000h`), ours into ecx; a named parent local is identical
// CTCTargetingPlayer::FrameUpdate() -- run the base update; drop a target that is dead, hidden or no longer Z-targetable while in targeting mode; age the target positions; refresh targeting every fourth frame (staggered by thing id) or on request; clear and recompute the projectile target in first-person; clear the target text with no target; update the display and the threat state; tick the scripted control
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00805600, 656 bytes (641 of code), plain `ret`; vtable slot FrameUpdate.
// Callees: CTCTargetingBase::FrameUpdate 0x006868A0 (catalog "$E2"), CMainGameComponent player-interface getter 0x00416359
//   (catalog "DrawGetEnvironment") on 0x013B86A0, CGamePlayerInterface::IsActive 0x00445B80 / IsGameActionEventInQueue
//   0x00446B10, CThing::GetPlayer 0x004C7A30, CPlayer player-number getter 0x00487510 / IsFirstPersonTargeting 0x004874D0,
//   const LowerBound 0x004365B0, CTCTargeted::IsZTargetable 0x007EA290, CBaseIntelligentPointer 0x00A01B10 / 0x00A01B50 /
//   0x00A01B90 / 0x00A01C10 (vtable 0x01238C6C), CWorld::GetWorldFrame 0x0049D870, UpdatePlayerTargeting 0x00804020,
//   UpdateProjectileTargeting 0x00803D60, 0x00803540 (first-person update), CWideString() 0x0099AED0 / ~ 0x0099B510, the
//   player GUI 0x013B8790 text setter 0x004379D0, UpdateTargetThreatening 0x00803280, CThing 0x0085EE10; vslots +0x84 /
//   +0x88 / +0x9c.
// Layout: engine/CTCTargetingPlayer.h is right to +0x68 but its two 16-byte PDB vectors are 12 bytes in retail, so from
//   +0x80 everything is 8 lower (TargetedPos +0x84, OldTargetedPos +0x90, offsets +0x9c / +0xa8, PLastThreatenedThing +0xd4,
//   PProjectileTargetedThing +0xdc); modelled locally. TargetedPoint +0x4c (pointer +0x4c, index +0x54).
// Shape: the base is polymorphic (a non-polymorphic base sits at +4 and costs an ebp this-adjust); pointer clears are
//   `P = CIntelligentPointer<CThing>(0)`; the GUI pointer is a named local read before the CWideString temporary; the
//   thing id is an 8-byte struct copy (+0x18/+0x1c) of which the first word staggers the update.
struct C3DVector { float X, Y, Z; };
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                     // vtable 0x01238C6C
    CIntelligentPointer() { }
    explicit CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
};
struct CWideString {
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
struct CPlayer {
    unsigned char _pad00[0x20a];
    bool          InTargetingMode;                          // +0x20a
    bool IsFirstPersonTargeting();                          // 0x004874D0
    long GetPlayerNumber();                                 // 0x00487510
};
struct CThing {
    unsigned char _pad00[0x18];
    struct SID { long A, B; } ID;                           // +0x18
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                                  // +0x91
    CPlayer* GetPlayer() const;                             // 0x004C7A30
    void UpdateScriptedControl();                           // 0x0085EE10
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CTCTargeted { bool IsZTargetable(); };              // 0x007EA290
struct CGamePlayerInterface {
    bool IsActive();                                        // 0x00445B80
    bool IsGameActionEventInQueue(long action, long type, long player);   // 0x00446B10
};
struct CMainGameComponent { CGamePlayerInterface* GetPlayerInterface(); };   // 0x00416359
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0
struct CPlayerGui { void SetTargetText(const CWideString& text); };   // 0x004379D0
extern CPlayerGui* g_pPlayerGui;                            // 0x013B8790
struct CWorld { static long GetWorldFrame(); };             // 0x0049D870

struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;                     // +0x00
    long          Index;                                    // +0x08
    unsigned char _pad0c[0x1c - 0x0c];
};
struct CTCTargetingBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTargetedThing();                    // slot 33 (+0x84)
    virtual void SetPTargetedThing(CThing* thing);          // slot 34 (+0x88)
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void UpdateTargetDisplay();                     // slot 39 (+0x9c)
    CThing*       ParentThing;                              // +0x04
    void FrameUpdate();                                     // 0x006868A0
};
struct CTCTargetingPlayer : CTCTargetingBase {
    unsigned char _pad08[0x38 - 0x08];
    long          ZLockTargetSelectionTimer;                // +0x38
    bool          TargetNextThing;                          // +0x3c
    bool          ForcedToHaveNoTarget;                     // +0x3d
    unsigned char _pad3e[2];
    long          TargetNextThingMethod;                    // +0x40
    unsigned char _pad44[0x4c - 0x44];
    CTargetPoint  TargetedPoint;                            // +0x4c
    unsigned char _pad68[0x84 - 0x68];
    C3DVector     TargetedPos;                              // +0x84
    C3DVector     OldTargetedPos;                           // +0x90
    C3DVector     TargetedPosOffset3D;                      // +0x9c
    C3DVector     OldTargetedPosOffset3D;                   // +0xa8
    unsigned char _padb4[0xd4 - 0xb4];
    CIntelligentPointer<CThing> PLastThreatenedThing;       // +0xd4
    CIntelligentPointer<CThing> PProjectileTargetedThing;   // +0xdc
    void UpdatePlayerTargeting();                           // 0x00804020
    void UpdateProjectileTargeting();                       // 0x00803D60
    void UpdateFirstPersonTargeting();                      // 0x00803540
    void UpdateTargetThreatening();                         // 0x00803280
    void FrameUpdate();
};

void CTCTargetingPlayer::FrameUpdate()
{
    CTCTargetingBase::FrameUpdate();
    if (ForcedToHaveNoTarget) {
        SetPTargetedThing(0);
        return;
    }
    CGamePlayerInterface* input = g_pMainGameComponent->GetPlayerInterface();
    if (input->IsActive() && ParentThing->GetPlayer()->InTargetingMode) {
        if (input->IsGameActionEventInQueue(0x4a, 2, ParentThing->GetPlayer()->GetPlayerNumber()))
            ++ZLockTargetSelectionTimer;
        else
            ZLockTargetSelectionTimer = 0;
    }
    bool inTargetingMode = ParentThing->GetPlayer()->InTargetingMode;
    CThing* target = GetPTargetedThing();
    if (target && !(target->Flags91 & 1)) {
        bool lose = true;
        if (!(target->Flags91 & 0x20) && target->HasTC(9)) {
            CTCTargeted* targeted = target->GetTC<CTCTargeted>(9);
            if (!inTargetingMode || ParentThing->HasTC(0x94) || targeted->IsZTargetable())
                lose = false;
        }
        if (lose) {
            TargetedPoint.PThing = CIntelligentPointer<CThing>(0);
            TargetedPoint.Index = -1;
            SetPTargetedThing(0);
            if (TargetNextThing && TargetNextThingMethod == 0)
                TargetNextThing = false;
        }
    }
    OldTargetedPos = TargetedPos;
    OldTargetedPosOffset3D = TargetedPosOffset3D;
    CThing::SID id = ParentThing->ID;
    if ((CWorld::GetWorldFrame() + id.A) % 4 == 0 || TargetNextThing)
        UpdatePlayerTargeting();
    PProjectileTargetedThing = CIntelligentPointer<CThing>(0);
    if (ParentThing->GetPlayer()->IsFirstPersonTargeting()) {
        UpdateProjectileTargeting();
        UpdateFirstPersonTargeting();
    }
    if (!GetPTargetedThing()) {
        CPlayerGui* gui = g_pPlayerGui;
        gui->SetTargetText(CWideString());
    }
    UpdateTargetDisplay();
    CThing* threatened = PLastThreatenedThing.GetPItem();
    if (GetPTargetedThing() != threatened)
        UpdateTargetThreatening();
    ParentThing->UpdateScriptedControl();
}
