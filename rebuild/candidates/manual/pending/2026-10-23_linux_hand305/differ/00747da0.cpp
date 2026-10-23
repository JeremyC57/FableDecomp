// Grade: DIFFER(988v1010) -- lane gap: retail rounds the four-second frame count with fadd 0.5 / fistp plus an integer compare of float bits (a hand-written inlined rounding helper), which also forces retail's ebp frame; ours is an esp frame with ftol
// Tried: one full decoded draft.
// CTCStealth::FrameUpdate() -- decide whether the creature may keep (or start) sneaking this frame, post the "sneaking and moving" deed after four seconds of sneaking movement, then update viewability, hearability and danger
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747DA0, 1010 bytes, ebp frame, `ret`.
// Callees: the graphic's offset getter (vslot +0x118, C3DVector by hidden pointer), CThing::GetPosition @ 0x004C73D0, the
//   thing's radius-at-point vslot +0x140, the current action's id @ 0x00692620, CThingCreatureBase::
//   IsPerformingASneakingIncompatibleAction @ 0x00662620, CThing::GetPlayer @ 0x004C7A30, LowerBound @ 0x0040F020 / 0x004365B0,
//   the mode manager's current mode @ 0x0073A6E0 (type through vslot 1), the script task IsRunning @ 0x007122C0, the player
//   interface's toggle test @ 0x00445BA0, CGamePlayerInterface::IsGameActionEventInQueue @ 0x00446B10, the player number getter
//   @ 0x00487510, CTCCreatureModeManager::RemoveMode @ 0x0073ABA0 / AddMode @ 0x0073AE10, CTCBase::GetWorld @ 0x00686A70,
//   CWorld::GetWorldFrame @ 0x0049D870, CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990 / RemoveDeedStillSearchingForWitnesses
//   @ 0x006C5920, CTCStealth::UpdateViewability @ 0x00747480 / UpdateHearability @ 0x007479B0 / UpdateDangerInfo @ 0x00747C70.
// Float pool: 1e-4f @ 0x0129BA3C, 4.0f @ 0x012689D8; frames per second @ 0x01375550; the sneak-toggle option byte @ 0x01375749.
// Layout: Ego_r.pdb CTCStealth (Sneaking +0x0f, SneakingToggleSet +0x10, FrameStartedWalking +0x20,
//   FrameStartedSneakingAndMoving +0x24, SneakingAndMovingDeedHandle +0x28, ActionPermittedToSneakID +0x2c); the creature's
//   velocity +0x10c/+0x110, current action +0x100, hero state +0x200; the player's no-sneak flag +0x20a, interface +0x0c.
// Note: retail rounds the four-second frame count with an fistp sequence plus an integer compare of float bits (an inline
//   rounding helper); plain C++ gives ftol.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    SEntry* LowerBound2(const long* type);              // 0x004365B0
};
struct CGraphic {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetOffset() const;                // +0x118
};
struct CGamePlayerInterface {
    bool IsSneakToggleMode();                           // 0x00445BA0
    bool IsGameActionEventInQueue(long action, long type, long player);   // 0x00446B10
};
struct CPlayer {
    unsigned char _pad00[0x0c];
    CGamePlayerInterface* Interface;                    // +0x0c
    unsigned char _pad10[0x20a - 0x10];
    bool          SneakDisabled;                        // +0x20a
    long GetNumber();                                   // 0x00487510
};
struct CAction { long GetID(); };                       // 0x00692620
struct CThing;
struct CTCHeroOpinionDeedLog {
    long PostDeed(long type, const C3DVector& pos, CThing* a, CThing* b, long post, bool c, bool d, void* faction, bool e,
                  long f);                              // 0x006C6990
    void RemoveDeedStillSearchingForWitnesses(long handle);   // 0x006C5920
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79();
    virtual float GetRadiusAt(const C3DVector* point);  // +0x140
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingType;                            // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CGraphic*     Graphic;                              // +0x60
    unsigned char _pad64[0x100 - 0x64];
    CAction*      PCurrentAction;                       // +0x100
    unsigned char _pad104[0x10c - 0x104];
    float         VelocityX;                            // +0x10c
    float         VelocityY;                            // +0x110
    unsigned char _pad114[0x200 - 0x114];
    long          HeroState;                            // +0x200
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CPlayer* GetPlayer();                               // 0x004C7A30
    bool IsPerformingASneakingIncompatibleAction();     // 0x00662620
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T>
    __forceinline T* GetTC2(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound2(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CMode { virtual void v00(); virtual long GetType(); };
struct CTCCreatureModeManager {
    CMode* GetCurrentMode();                            // 0x0073A6E0
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
    void AddMode(unsigned long mode);                   // 0x0073AE10
};
struct CScriptTask {
    unsigned char _pad00[0x18];
    bool          Paused;                               // +0x18
    bool IsRunning();                                   // 0x007122C0
};
struct CWorld { static long GetWorldFrame(); };          // 0x0049D870
extern long g_framesPerSecond;                          // 0x01375550
extern bool g_sneakToggleOption;                        // 0x01375749

struct CTCStealth {
    void*   __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0f - 0x08];
    bool    Sneaking;                                   // +0x0f
    bool    SneakingToggleSet;                          // +0x10
    unsigned char _pad11[0x20 - 0x11];
    long    FrameStartedWalking;                        // +0x20
    long    FrameStartedSneakingAndMoving;              // +0x24
    long    SneakingAndMovingDeedHandle;                // +0x28
    long    ActionPermittedToSneakID;                   // +0x2c
    CWorld* GetWorld();                                 // 0x00686A70
    void UpdateViewability();                           // 0x00747480
    void UpdateHearability();                           // 0x007479B0
    void UpdateDangerInfo();                            // 0x00747C70
    void FrameUpdate();
};

void CTCStealth::FrameUpdate()
{
    CThing* creature = ParentThing;
    float speedSq = creature->VelocityX * creature->VelocityX + creature->VelocityY * creature->VelocityY;
    C3DVector centre = creature->Graphic->GetOffset() + creature->GetPosition();
    float radius = creature->GetRadiusAt(&centre) + 1e-4f;
    float radiusSq = radius * radius;
    bool canSneak = true;
    if (creature->PCurrentAction && creature->PCurrentAction->GetID() != ActionPermittedToSneakID) {
        if (creature->IsPerformingASneakingIncompatibleAction())
            canSneak = false;
        ActionPermittedToSneakID = -1;
    }
    if (creature->GetPlayer()->SneakDisabled)
        canSneak = false;
    CTCCreatureModeManager* modes = 0;
    if (ParentThing->HasTC(0x31)) {
        modes = ParentThing->GetTC<CTCCreatureModeManager>(0x31);
        long mode = modes->GetCurrentMode()->GetType();
        if (!Sneaking && mode != 1 && mode != 4)
            canSneak = false;
    }
    if (ParentThing->HasTC(0x1f)) {
        CScriptTask* task = ParentThing->GetTC2<CScriptTask>(0x1f);
        canSneak = canSneak && !task->Paused && task->IsRunning();
    }
    if (ParentThing->ThingType == 2) {
        bool wants;
        if (ParentThing->GetPlayer()->Interface->IsSneakToggleMode())
            wants = SneakingToggleSet;
        else
            wants = ParentThing->GetPlayer()->Interface->IsGameActionEventInQueue(0x20, 2, ParentThing->GetPlayer()->GetNumber());
        if (!wants)
            canSneak = false;
    }
    if (Sneaking) {
        bool keep;
        if (ParentThing->GetPlayer()->Interface->IsSneakToggleMode())
            keep = SneakingToggleSet;
        else if (ParentThing->ThingType == 2)
            keep = ParentThing->HeroState != 3 && (ParentThing->HeroState != 4 || g_sneakToggleOption);
        else
            keep = !(radiusSq - 1e-4f <= speedSq);
        if (!keep || !canSneak) {
            if (modes)
                modes->RemoveMode(7);
            Sneaking = false;
            FrameStartedWalking = FrameStartedSneakingAndMoving = -1;
        }
        else if (speedSq > 1e-4f) {
            if (FrameStartedSneakingAndMoving == -1)
                FrameStartedSneakingAndMoving = GetWorld()->GetWorldFrame();
            else if (SneakingAndMovingDeedHandle == -1) {
                GetWorld();
                long frames = (long)(g_framesPerSecond * 4.0f);
                if (CWorld::GetWorldFrame() - FrameStartedSneakingAndMoving > frames && creature->HasTC(0x51)) {
                    CTCHeroOpinionDeedLog* log = creature->GetTC<CTCHeroOpinionDeedLog>(0x51);
                    SneakingAndMovingDeedHandle = log->PostDeed(0x49, creature->GetPosition(), 0, 0, 2, 0, 1, 0, 0, 0);
                }
            }
        }
        else
            FrameStartedSneakingAndMoving = -1;
    }
    else {
        bool option = g_sneakToggleOption;
        if ((ParentThing->GetPlayer()->Interface->IsSneakToggleMode() || option || !(radiusSq - 1e-4f <= speedSq)) && canSneak) {
            if (modes)
                modes->AddMode(7);
            Sneaking = true;
        }
        else
            FrameStartedWalking = -1;
    }
    if (!Sneaking && SneakingAndMovingDeedHandle != -1 && creature->HasTC(0x51)) {
        creature->GetTC<CTCHeroOpinionDeedLog>(0x51)->RemoveDeedStillSearchingForWitnesses(SneakingAndMovingDeedHandle);
        SneakingAndMovingDeedHandle = -1;
    }
    UpdateViewability();
    UpdateHearability();
    UpdateDangerInfo();
}
