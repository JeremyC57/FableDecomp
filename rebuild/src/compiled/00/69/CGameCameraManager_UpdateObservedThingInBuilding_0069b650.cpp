// CGameCameraManager::UpdateObservedThingInBuilding() -- when the current mode is interruptable (or a mode is pending), push the building camera mode once the observed thing is alive inside a live building, or remove it once it is not
// Leaf: ?UpdateObservedThingInBuilding@CGameCameraManager@@QAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069B650, length 152 bytes (+ int3 padding).
// Callees: IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase>) @ 0x00697A50 (static `__stdcall`, by-value copy built in
// its argument slot), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, on PObservedThing), GetPBuildingThingOccupying(CThing*)
// @ 0x006989D0 (`__fastcall`), PushBuildingCameraMode @ 0x0069AD10 and RemoveBuildingCameraMode @ 0x0069B610 (landed) as sibling tail jumps.
// Layout: retail CGameCameraManager as the landed UpdateObservedThingInMelee 0x0069AC30 (CurrentMode +0x98, PendingMode +0xa0,
// ObservedThingInBuilding +0xaa) with PObservedThing +0x100.
// Shape: the twin of UpdateObservedThingInMelee: early return unless interruptable or pending, then the alive tests of the thing and
// of its building in one `&&` chain; ObservedThingInBuilding read in each arm.
#include "engine/CThing.h"
class CThingBuilding : public CThing { public: };
CThingBuilding* __fastcall GetPBuildingThingOccupying(CThing* thing);   // 0x006989D0

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace NCameraModes { class CBase; }
#include "engine/CCountedPointer.h"

class CGameCameraManager { public:
    void*         __vftable;                                // +0x00
    unsigned char _pad04[0x94];                             // +0x04
    CCountedPointer<NCameraModes::CBase> CurrentMode;       // +0x98
    CCountedPointer<NCameraModes::CBase> PendingMode;       // +0xa0
    bool          NewAutoMode;                              // +0xa8
    bool          ObservedThingInMelee;                     // +0xa9
    bool          ObservedThingInBuilding;                  // +0xaa
    unsigned char _padab[0x55];                             // +0xab
    CIntelligentPointer<CThing> PObservedThing;                 // +0x100
    static bool __stdcall IsCameraModeInterruptable(CCountedPointer<NCameraModes::CBase> mode);   // 0x00697A50
    void PushBuildingCameraMode();                          // 0x0069AD10
    void RemoveBuildingCameraMode();                        // 0x0069B610
    void UpdateObservedThingInBuilding();
};

void CGameCameraManager::UpdateObservedThingInBuilding()
{
    if (!IsCameraModeInterruptable(CurrentMode) && PendingMode.Data == 0)
        return;
    CThingBuilding* building;
    if (PObservedThing.GetPItem() && !PObservedThing.GetPItem()->IsDead()
        && (building = GetPBuildingThingOccupying(PObservedThing.GetPItem())) != 0 && !building->IsDead()) {
        if (!ObservedThingInBuilding)
            PushBuildingCameraMode();
    } else {
        if (ObservedThingInBuilding)
            RemoveBuildingCameraMode();
    }
}