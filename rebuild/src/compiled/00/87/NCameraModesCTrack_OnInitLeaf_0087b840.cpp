// NCameraModes::CTrack::OnInitLeaf() -- when the target's shape manager has a best track, start at the track point nearest the target's track camera point
// Leaf: ?OnInitLeaf@CTrack@NCameraModes@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B840, 199 bytes, plain `ret`. Ego_r.pdb ?OnInitLeaf@CTrack@NCameraModes@@UAEXXZ (public virtual).
// Callees: CBaseIntelligentPointer::GetPItem const 0x00A01B50 (operator-> on the target, twice); CVectorMap<ETCInterfaceType,CTCBase*>
//   ::LowerBound const 0x004365B0 (inside CThing::PeekTC<const T>); CTCShapeManager::GetBestTrack const 0x00761B50 (Ego_r.pdb
//   QBE_NPAPBVCTrack@NShape@@); CTCCameraPointDefinitionBase::GetCameraPointType (virtual slot 28, +0x70) inside the inline
//   GetPointType<CTCCameraPointTrack> (Ego_r.pdb ??$GetPointType@VCTCCameraPointTrack@@@..QBE_NPAPBV..); NShape::CTrack::
//   GetTrackPosNearest const 0x00760DA0 (Ego_r.pdb QBEXABVC3DVector@@AAV3@AAM@Z).
// Shape: both lookups' results are used unchecked, as retail does (an absent component or a non-track point leaves the out pointer
//   unset; retail reads the shared out slot). Layout: target +0xA4 (as the landed UpdateDesiredPos 0x0087B910), PTrack +0xC0,
//   CurrentFractionAlongTrack +0xC4 (Ego_r.pdb +0xE4 / +0xE8); CTCCameraPointTrack DragCamPos +0x48 (Ego_r.pdb).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTCSHAPEMANAGER_TRACK
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCShapeManager.h"
namespace NShape {
class CTrack {
public:
    void GetTrackPosNearest(const C3DVector& pos, C3DVector& nearest, float& fraction) const;   // 0x00760DA0
};
}
enum ECameraPointType { CAMERA_POINT_TYPE_TRACK = 3 };
class CTCCameraPointDefinitionBase : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CAMERA_POINT; }
    virtual ECameraPointType GetCameraPointType() const = 0;   // slot 28 (+0x70)
    template<class T> bool GetPointType(const T** point) const
    {
        if (GetCameraPointType() == T::StatGetCameraPointType()) {
            *point = static_cast<const T*>(this);
            return true;
        }
        return false;
    }
    unsigned char _pad_after_base[0x40 - sizeof(CTCBase)];   // CutInto +0x0c .. Activated +0x3c (Ego_r.pdb)
};
class CTCCameraPointTrack : public CTCCameraPointDefinitionBase {
public:
    static ECameraPointType __fastcall StatGetCameraPointType() { return CAMERA_POINT_TYPE_TRACK; }
    float CameraStringLength;                   // +0x40
    float CameraCageRadius;                     // +0x44
    C3DVector DragCamPos;                       // +0x48
};
namespace NCameraModes {
class CTrack {
public:
    virtual void OnInitLeaf();                              // (its real vtable slot is not needed here)
    unsigned char               _pad04[0xa0];
    CIntelligentPointer<CThing> m_target;                   // +0xa4
    unsigned char               _padac[0x14];
    const NShape::CTrack*       PTrack;                     // +0xc0
    float                       CurrentFractionAlongTrack;  // +0xc4
    bool                        ZTargeting;                 // +0xc8
};
}

void NCameraModes::CTrack::OnInitLeaf()
{
    const CTCShapeManager* shapeManager;
    m_target->PeekTC(&shapeManager);
    if (shapeManager->GetBestTrack(&PTrack)) {
        const CTCCameraPointDefinitionBase* cameraPoint;
        m_target->PeekTC(&cameraPoint);
        const CTCCameraPointTrack* trackPoint;
        cameraPoint->GetPointType(&trackPoint);
        C3DVector nearest;
        PTrack->GetTrackPosNearest(trackPoint->DragCamPos, nearest, CurrentFractionAlongTrack);
    }
}