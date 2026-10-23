// Grade: DIFFER(848v844) -- control flow, frame, calls, CGameEvent and both sounds match; three register residues: camera-point vcall loads the component into eax then ecx (retail straight into ecx), GetCameraManager eax/edx swap, and retail keeps the mode Object in esi across SetTrack (CSE) where we reload it; tried GetWorld wrapper (fixed ecx), local-copy counted-pointer copy ctor (fixed shape), pointer-returning and entry-level wrappers for the vcall, template converting copy
// CTCActionUseScriptedHook::GlobalUpdate(...) -- while a confirmation is pending consume the confirm message (setting PerformAction on a yes); otherwise perform the queued action: teleport the hero through the connected entrance (door-latch sound, or the locked-door sound), then start a camera-track scripted camera mode when the track's camera point is of type 7
// Leaf: GlobalUpdate
// Flags: /GS
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
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
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;                   // slot 63 (+0xfc)
};
struct CTCSound {
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
struct CTCCameraPoint {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType() const;            // slot 28 (+0x70)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 the interface-type bitset
    struct CInterfaceMap_Retail {
        struct SEntry { long type; void* component; };
        SEntry* first;
        SEntry* last;
        SEntry* LowerBound(const long* type);           // 0x0040F020
    } m_interfaces;                                     // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char _pad64[0x90 - 0x64];
    signed char Player;                                 // +0x90
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    float GetAngleXY() const { return PhysicsTC->GetAngleXY(); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* PData;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CPlayer { CThing* GetHeroThing(); };             // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449970
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFLOAT(float value) { Copy(4, &value); }
    void AddBOOL(bool value) { Copy(1, &value); }
};
struct CMessageEvent {
    unsigned char _pad[0x3c];
    long Answer;                                        // +0x3c
};
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
struct CCPPointerInfo { long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
template <class T> class CCountedPointer {
public:
    T*              Object;
    CCPPointerInfo* Info;
    CCountedPointer(T* object);                         // 0x005ED270 (out of line)
    CCountedPointer(const CCountedPointer& other)
    {
        T* object = other.Object;
        CCPPointerInfo* info = other.Info;
        Object = object;
        Info = info;
        if (info)
            ++info->RefCount;
    }
    ~CCountedPointer();                                 // 0x005ED2C0
    T* operator->() const { return Object; }
};
namespace NCameraModes {
class CScriptModuleCameraTrack {                        // 0x64 bytes
public:
    unsigned char _body[0x64];
    CScriptModuleCameraTrack(long a, long b);           // 0x00852710
    void SetTrack(CThing* track);                       // 0x0072A4A0
};
}
class CGameCameraManager {
public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleCameraTrack> mode);   // 0x00699E60
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;              // +0x34
    unsigned char _pad38[0x60 - 0x38];
    CMessageEventManager* PMessageEventManager;         // +0x60
    bool GetCameraManager(CGameCameraManager** manager) { return PCurrentGameCamera->GetCameraManager(manager); }
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);   // 0x0049EAF0
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CWorld* GetWorld();                                 // 0x00686A70
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
};
struct CIntelligentPointerThing : CBaseIntelligentPointer { };
static __forceinline long GetPointType(CTCCameraPoint* point) { return point->GetCameraPointType(); }
class CTCActionUseScriptedHook : public CTCBase {
public:
    bool Usable;                                        // +0x0c
    bool ForceConfirmation;                             // +0x0d
    bool WaitingForConfirmation;                        // +0x0e
    bool PerformAction;                                 // +0x0f
    bool TeleportToRegionEntrance;                      // +0x10
    unsigned char _pad11[7];
    unsigned __int64 EntranceConnectedToUID;            // +0x18
    CIntelligentPointerThing PEntranceConnectedTo;      // +0x20
    bool EntranceLocked;                                // +0x28
    unsigned char _pad29[7];
    unsigned __int64 CameraTrackUID;                    // +0x30
    CIntelligentPointerThing PCameraTrack;              // +0x38
    void GlobalUpdate(long frame);
};

void CTCActionUseScriptedHook::GlobalUpdate(long frame)
{
    if (ForceConfirmation && WaitingForConfirmation) {
        CMessageEventManager* manager = GetWorld()->PMessageEventManager;
        CMessageEvent* message = manager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x10));
        if (message) {
            if (message->Answer == 1)
                PerformAction = true;
            WaitingForConfirmation = false;
            manager->RemoveMessage(message);
        }
        return;
    }
    if (!PerformAction)
        return;
    if (TeleportToRegionEntrance && PEntranceConnectedTo.GetPItem()) {
        CTCSound* sound = 0;
        ParentThing->GetInterface(&sound, 0x10);
        if (EntranceLocked) {
            if (sound)
                sound->PlaySound("DOOR_OPEN_FAIL", 0, 0, 0, -1);
        } else {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
            if (hero && !hero->Dead) {
                CGameEvent event(0x2c);
                event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetPosition().X);
                event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetPosition().Y);
                event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetPosition().Z);
                event.AddFLOAT(PEntranceConnectedTo.GetPItem()->GetAngleXY());
                event.AddBOOL(false);
                event.AddBOOL(false);
                event.AddBOOL(true);
                event.Player = hero->Player;
                GetWorld()->HandleMoveHeroToRegionGameEvent(event);
                sound->PlaySound("BIG_DOOR_UNLATCH", 1, 0, 0, -1);
            }
        }
    }
    CTCCameraPoint* point;
    if (PCameraTrack.GetPItem() && PCameraTrack.GetPItem()->GetInterface(&point, 0x3f) && GetPointType(point) == 7) {
        CGameCameraManager* cameraManager = 0;
        GetWorld()->GetCameraManager(&cameraManager);
        CCountedPointer<NCameraModes::CScriptModuleCameraTrack> mode(new NCameraModes::CScriptModuleCameraTrack(0, -1));
        CThing* track = PCameraTrack.GetPItem();
        mode->SetTrack(track);
        cameraManager->SetScriptedCameraMode(mode);
    }
    PerformAction = false;
}
