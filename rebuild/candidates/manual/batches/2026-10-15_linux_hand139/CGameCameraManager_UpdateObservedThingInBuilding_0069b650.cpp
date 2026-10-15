// CGameCameraManager::UpdateObservedThingInBuilding() -- when the current mode is interruptable (or a mode is pending), push the building camera mode once the observed thing is alive inside a live building, or remove it once it is not
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069B650, length 152 bytes (+ int3 padding).
// Callees: IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase>) @ 0x00697A50 (static `__stdcall`, by-value copy built in
// its argument slot), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on PObservedThing), GetPBuildingThingOccupying(CThing*)
// @ 0x006989D0 (`__fastcall`), PushBuildingCameraMode @ 0x0069AD10 and RemoveBuildingCameraMode @ 0x0069B610 (landed) as sibling tail jumps.
// Layout: retail CGameCameraManager as the landed UpdateObservedThingInMelee 0x0069AC30 (CurrentMode +0x98, PendingMode +0xa0,
// ObservedThingInBuilding +0xaa) with PObservedThing +0x100.
// Shape: the twin of UpdateObservedThingInMelee: early return unless interruptable or pending, then the alive tests of the thing and
// of its building in one `&&` chain; ObservedThingInBuilding read in each arm.
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_statusFlags;            // +0x91 (bit 0: dead)
};
struct CThingBuilding : CThing { };
CThingBuilding* __fastcall GetPBuildingThingOccupying(CThing* thing);   // 0x006989D0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

namespace NCameraModes { struct CBase; }
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};
template <class T>
struct CCountedPointer {
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};

struct CGameCameraManager {
    void*         __vftable;                                // +0x00
    unsigned char _pad04[0x94];                             // +0x04
    CCountedPointer<NCameraModes::CBase> CurrentMode;       // +0x98
    CCountedPointer<NCameraModes::CBase> PendingMode;       // +0xa0
    bool          NewAutoMode;                              // +0xa8
    bool          ObservedThingInMelee;                     // +0xa9
    bool          ObservedThingInBuilding;                  // +0xaa
    unsigned char _padab[0x55];                             // +0xab
    CBaseIntelligentPointer PObservedThing;                 // +0x100
    static bool __stdcall IsCameraModeInterruptable(CCountedPointer<NCameraModes::CBase> mode);   // 0x00697A50
    void PushBuildingCameraMode();                          // 0x0069AD10
    void RemoveBuildingCameraMode();                        // 0x0069B610
    void UpdateObservedThingInBuilding();
};

void CGameCameraManager::UpdateObservedThingInBuilding()
{
    if (!IsCameraModeInterruptable(CurrentMode) && PendingMode.m_pObject == 0)
        return;
    CThingBuilding* building;
    if (PObservedThing.GetPItem() && !(PObservedThing.GetPItem()->m_statusFlags & 1)
        && (building = GetPBuildingThingOccupying(PObservedThing.GetPItem())) != 0 && !(building->m_statusFlags & 1)) {
        if (!ObservedThingInBuilding)
            PushBuildingCameraMode();
    } else {
        if (ObservedThingInBuilding)
            RemoveBuildingCameraMode();
    }
}
