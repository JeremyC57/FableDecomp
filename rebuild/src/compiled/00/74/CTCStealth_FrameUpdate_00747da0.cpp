// CTCStealth::FrameUpdate() -- decide whether the creature may keep (or start) sneaking this frame, post the "sneaking and moving" deed after four seconds of sneaking movement, then update viewability, hearability and danger
// Grade: RELOCATION_MATCH (retry wave 2 sec2c, 9 grades): the four-second frame count goes through the engine's inline-assembly FPU ceiling helper (the landed CeilToLong of 0x0079C5F0), which also gives retail's ebp frame; the radius is taken at the temporary (pos + graphic offset) passed straight to the vslot; the sneak-keep tests clear canSneak in place (one removal block); the deed handle is read inside the !Sneaking block; PDB spellings for the callees
// Retail-asm: 0x0074803F `fstp [ebp-8]` / 0x00748042 `fld [ebp-8]` + 0x00748045 `fadd [ebp-10h]` + 0x00748048 `fistp dword ptr [ebp-0Ch]` + 0x0074804B `fild` / 0x0074804E `fsub [ebp-14h]` / 0x00748054 `mov eax,[ebp-8] ; cmp eax,[ebp-18h] ; jne ; dec dword ptr [ebp-0Ch]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants at 0x0074802B / 0x00748032, a bare dword fistp into a named long). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`); same 10-instruction block as the landed retail_asm 0x0079C5F0 / standalone 0x004FA9A0.
// Name: CTCStealth::FrameUpdate is virtual (Ego_r.pdb ?FrameUpdate@CTCStealth@@UAEXXZ); 0x00747DA0 sits in the CTCStealth vtable at 0x01268A10 (RTTI COL 0x01343474 -> .?AVCTCStealth@@), next to HasFrameUpdate 0x00749410 (`mov al,1 ; ret`).
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747DA0, 1010 bytes, ebp frame, `ret`.
// Callees: the graphic's offset getter (vslot +0x118, C3DVector by hidden pointer), CThing::GetPos @ 0x004C73D0, the
//   thing's radius-at-point vslot +0x140, CCreatureActionBase::GetActionID @ 0x00692620, CThingCreatureBase::
//   IsPerformingASneakingIncompatibleAction @ 0x00662620, CThing::GetPlayer @ 0x004C7A30, LowerBound @ 0x0040F020 / 0x004365B0,
//   the mode manager's current mode @ 0x0073A6E0 (type through vslot 1), the TC-0x1F component's list-empty test @ 0x007122C0 (catalog CTCScriptedControl::IsActionListEmpty; layout unconfirmed, kept descriptive), the player
//   interface's CGamePlayerInterface::IsToggleSneak @ 0x00445BA0, CGamePlayerInterface::IsGameActionEventInQueue @ 0x00446B10, CPlayer::GetJoystickDeviceNumber
//   @ 0x00487510, CTCCreatureModeManager::RemoveMode @ 0x0073ABA0 / AddMode @ 0x0073AE10, CTCBase::GetWorld @ 0x00686A70,
//   CWorld::GetFrame @ 0x0049D870 (static), CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990 / RemoveDeedStillSearchingForWitnesses
//   @ 0x006C5920, CTCStealth::UpdateViewability @ 0x00747480 / UpdateHearability @ 0x007479B0 / UpdateDangerInfo @ 0x00747C70.
// Float pool: 1e-4f @ 0x0129BA3C, 4.0f @ 0x012689D8; frames per second @ 0x01375550; the sneak-toggle option byte @ 0x01375749.
// Layout: Ego_r.pdb CTCStealth (Sneaking +0x0f, SneakingToggleSet +0x10, FrameStartedWalking +0x20,
//   FrameStartedSneakingAndMoving +0x24, SneakingAndMovingDeedHandle +0x28, ActionPermittedToSneakID +0x2c); the creature's
//   velocity +0x10c/+0x110, current action +0x100, hero state +0x200; the player's no-sneak flag +0x20a, interface +0x0c.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CTCBase;
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
enum EGameAction { GAME_ACTION_SNEAK_0x20 = 0x20 };
enum EInputButtonEventType { INPUT_BUTTON_EVENT_0x2 = 2 };
class CGamePlayerInterface { public:
    bool IsToggleSneak() const;                         // 0x00445BA0
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long player) const;   // 0x00446B10
};
class CPlayer { public:
    unsigned char _pad00[0x0c];
    CGamePlayerInterface* Interface;                    // +0x0c
    unsigned char _pad10[0x20a - 0x10];
    bool          SneakDisabled;                        // +0x20a
    long GetJoystickDeviceNumber() const;               // 0x00487510
};
class CCreatureActionBase { public: long GetActionID() const; };   // 0x00692620
class CThing;
class CTCHeroOpinionDeedLog { public:
    long PostDeed(long type, const C3DVector& pos, CThing* a, CThing* b, long post, bool c, bool d, void* faction, bool e,
                  long f);                              // 0x006C6990
    void RemoveDeedStillSearchingForWitnesses(long handle);   // 0x006C5920
};
class CThing { public:
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
    virtual float GetRadiusAt(const C3DVector& point);  // +0x140
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingType;                            // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CGraphic*     Graphic;                              // +0x60
    unsigned char _pad64[0x100 - 0x64];
    CCreatureActionBase* PCurrentAction;                       // +0x100
    unsigned char _pad104[0x10c - 0x104];
    float         VelocityX;                            // +0x10c
    float         VelocityY;                            // +0x110
    unsigned char _pad114[0x200 - 0x114];
    long          HeroState;                            // +0x200
    const C3DVector& GetPos() const;                    // 0x004C73D0
    CPlayer& GetPlayer();                               // 0x004C7A30
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
class CTCCreatureModeManager { public:
    class CModeBase { public: virtual void v00(); virtual long GetType() const; };   // mode type through vslot 1

    CModeBase& GetCurrentMode() const;                  // 0x0073A6E0
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};
class CThingCreatureBase : public CThing { public:
    bool IsPerformingASneakingIncompatibleAction() const;   // 0x00662620
};
struct CScriptTask {
    unsigned char _pad00[0x18];
    bool          Paused;                               // +0x18
    bool IsRunning();                                   // 0x007122C0
};
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870
extern long g_framesPerSecond;                          // 0x01375550
extern bool g_sneakToggleOption;                        // 0x01375749

// the engine's inline-assembly FPU ceiling helper (standalone copy 0x004FA9A0; landed inlined form 0x0079C5F0)
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CWorld& GetWorld() const;                           // 0x00686A70
};
class CTCStealth : public CTCBase { public:
    bool    Viewable;                                   // +0x0c
    bool    Smellable;                                  // +0x0d
    bool    Hearable;                                   // +0x0e
    bool    Sneaking;                                   // +0x0f
    bool    SneakingToggleSet;                          // +0x10
    float   DistanceCanBeHeardFrom;                     // +0x14
    float   MaxSneakingSpeed;                           // +0x18
    void*   PDef;                                       // +0x1c CDefPointer<const CStealthDef>
    long    FrameStartedWalking;                        // +0x20
    long    FrameStartedSneakingAndMoving;              // +0x24
    long    SneakingAndMovingDeedHandle;                // +0x28
    long    ActionPermittedToSneakID;                   // +0x2c
    virtual void FrameUpdate();
private:
    void UpdateViewability();                           // 0x00747480
    void UpdateHearability();                           // 0x007479B0
    void UpdateDangerInfo();                            // 0x00747C70
};

void CTCStealth::FrameUpdate()
{
    CThing* creature = ParentThing;
    float speedSq = creature->VelocityX * creature->VelocityX + creature->VelocityY * creature->VelocityY;
    float radius = creature->GetRadiusAt(creature->GetPos() + creature->Graphic->GetOffset()) + 1e-4f;
    bool canSneak = true;
    float radiusSq = radius * radius;
    if (creature->PCurrentAction && creature->PCurrentAction->GetActionID() != ActionPermittedToSneakID) {
        if (static_cast<CThingCreatureBase*>(creature)->IsPerformingASneakingIncompatibleAction())
            canSneak = false;
        ActionPermittedToSneakID = -1;
    }
    if (creature->GetPlayer().SneakDisabled)
        canSneak = false;
    CTCCreatureModeManager* modes = 0;
    if (ParentThing->HasTC(0x31)) {
        modes = ParentThing->GetTC<CTCCreatureModeManager>(0x31);
        long mode = modes->GetCurrentMode().GetType();
        if (!Sneaking && mode != 1 && mode != 4)
            canSneak = false;
    }
    if (ParentThing->HasTC(0x1f)) {
        CScriptTask* task = ParentThing->GetTC2<CScriptTask>(0x1f);
        canSneak = canSneak && !task->Paused && task->IsRunning();
    }
    if (ParentThing->ThingType == 2) {
        bool wants;
        if (ParentThing->GetPlayer().Interface->IsToggleSneak())
            wants = SneakingToggleSet;
        else
        {
            CThing* parent = ParentThing;
            CGamePlayerInterface* iface = parent->GetPlayer().Interface;
            wants = iface->IsGameActionEventInQueue(GAME_ACTION_SNEAK_0x20, INPUT_BUTTON_EVENT_0x2, parent->GetPlayer().GetJoystickDeviceNumber());
        }
        if (!wants)
            canSneak = false;
    }
    if (Sneaking) {
        if (ParentThing->GetPlayer().Interface->IsToggleSneak()) {
            if (!SneakingToggleSet)
                canSneak = false;
        }
        else if (ParentThing->ThingType == 2) {
            if (ParentThing->HeroState == 3 || (ParentThing->HeroState == 4 && !g_sneakToggleOption))
                canSneak = false;
        }
        else if (radiusSq - 1e-4f <= speedSq)
            canSneak = false;
        if (!canSneak) {
            if (modes)
                modes->RemoveMode(7);
            Sneaking = false;
            FrameStartedSneakingAndMoving = FrameStartedWalking = -1;
        }
        else if (speedSq > 1e-4f) {
            if (FrameStartedSneakingAndMoving == -1)
                FrameStartedSneakingAndMoving = GetWorld().GetFrame();
            else if (SneakingAndMovingDeedHandle == -1) {
                GetWorld();
                long frames = CeilToLong(g_framesPerSecond * 4.0f);
                if (CWorld::GetFrame() - FrameStartedSneakingAndMoving > frames && creature->HasTC(0x51)) {
                    CTCHeroOpinionDeedLog* log = creature->GetTC<CTCHeroOpinionDeedLog>(0x51);
                    SneakingAndMovingDeedHandle = log->PostDeed(0x49, creature->GetPos(), 0, 0, 2, 0, 1, 0, 0, 0);
                }
            }
        }
        else
            FrameStartedSneakingAndMoving = -1;
    }
    else {
        bool option = g_sneakToggleOption;
        if ((ParentThing->GetPlayer().Interface->IsToggleSneak() || option || radiusSq - 1e-4f > speedSq) && canSneak) {
            if (modes)
                modes->AddMode(7);
            Sneaking = true;
        }
        else
            FrameStartedWalking = -1;
    }
    if (!Sneaking) {
        long handle = SneakingAndMovingDeedHandle;
        if (handle != -1 && creature->HasTC(0x51)) {
            creature->GetTC<CTCHeroOpinionDeedLog>(0x51)->RemoveDeedStillSearchingForWitnesses(handle);
            SneakingAndMovingDeedHandle = -1;
        }
    }
    UpdateViewability();
    UpdateHearability();
    UpdateDangerInfo();
}