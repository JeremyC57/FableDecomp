// CGameCameraManager::PushBuildingCameraMode() -- the observed thing entered a building: drop the player's mode 0x2F,
// switch to the building's camera manager set (and back into melee mode when fighting), and flag the camera as in a
// building
// Leaf: PushBuildingCameraMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069AD10, length 176 bytes, `ret`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::RemoveMode(EPlayerMode) @ 0x00634190,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the `__fastcall` GetPBuildingThingOccupying(CThing*) @ 0x006989D0,
// CThingBuilding::PeekPCameraManagerSetDefinition() (CDefPointer by hidden pointer) @ 0x0082D3B0,
// CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<const CCameraManagerSetDef>&) @ 0x0069AC90,
// IsObservedThingInMelee @ 0x006994F0, AddMeleeCameraMode @ 0x0069A8B0 (labelled CTCCombat::AddMeleeFlourishAbility),
// the def's virtual slot 1 on release. Global: the engine 0x013B86A0 (player manager +0x1C).
// Retail layout: PObservedThing +0x100, ObservedThingInBuilding +0xAA (engine/CGameCameraManager.h is 0x4..0x18
// higher; see the landed RemoveBuildingCameraMode 0x0069B610); CThing dead bit +0x91.
// Shape: the def pointer local's dtor is the null-tested inline ReleaseTemporary (DecRef / GetRefCount on the
// ref-count base at +4, landed 0x00702630 model); ObservedThingInBuilding = true is the last statement (VC duplicates
// it per epilogue and shares the constant 1 with the dead-bit test).

class CPlayer {
public:
    void RemoveMode(long mode);                     // 0x00634190
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                       // 0x00449970 (the non-const overload; 0x00449960 is the const one)
};
class CEngine { public:
    unsigned char _pad[0x1c];
    CPlayerManager* m_pPlayerManager;               // +0x1C
};
extern CEngine* g_pEngine;                          // 0x013B86A0

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                         // slot 1
};
class CCameraManagerSetDef : public CRefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
};

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
class CThingBuilding : public CThing { public:
    CDefPointer<CCameraManagerSetDef> PeekPCameraManagerSetDefinition();    // 0x0082D3B0
};
CThingBuilding* __fastcall GetPBuildingThingOccupying(CThing* thing);       // 0x006989D0

class CBaseObject;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CGameCameraManager {
public:
    virtual void v00();
    unsigned char _pad004[0xa6];
    bool ObservedThingInBuilding;                   // +0xAA
    unsigned char _pad0ab[0x55];
    CIntelligentPointer<CThing> PObservedThing;     // +0x100
    void SetCameraManagerSetDef(const CDefPointer<CCameraManagerSetDef>& def);  // 0x0069AC90
    bool IsObservedThingInMelee();                  // 0x006994F0
    void AddMeleeCameraMode();                      // 0x0069A8B0
    void PushBuildingCameraMode();
};

void CGameCameraManager::PushBuildingCameraMode()
{
    g_pEngine->m_pPlayerManager->GetMainPlayer().RemoveMode(0x2f);
    if (PObservedThing.GetPItem()) {
        CThingBuilding* building = GetPBuildingThingOccupying(PObservedThing.GetPItem());
        if (building) {
            if (!building->m_dead) {
                CDefPointer<CCameraManagerSetDef> def = building->PeekPCameraManagerSetDefinition();
                SetCameraManagerSetDef(def);
                if (IsObservedThingInMelee())
                    AddMeleeCameraMode();
            }
        }
    }
    ObservedThingInBuilding = true;
}