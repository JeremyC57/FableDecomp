// CTCCameraPointTrack::GetActiveCameraPosition() const -- the camera position: the point of the parent's best track nearest the drag-camera position
// Leaf: ?GetActiveCameraPosition@
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075DC60, 129 bytes (+int3 pad to 144), `ret 4` (C3DVector returned
//   through the hidden pointer).
// Identity: CTCCameraPointTrack vtable 0x0123B0E4 slot 35 (RTTI .?AVCTCCameraPointTrack@@); Ego_r.pdb
//   ?GetActiveCameraPosition@CTCCameraPointTrack@@MBE?AVC3DVector@@XZ overrides the CTCCameraPointDefinitionBase slot
//   (the manifest's "CTCObject_GetTrackPosition" is a sibling-vtable autoname).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0, CTCShapeManager::GetBestTrack
//   (const NShape::CTrack**) const @ 0x00761B50 (result ignored), NShape::CTrack::GetTrackPosNearest(const C3DVector&,
//   C3DVector&, float&) const @ 0x00760DA0.
// Shape: the inline out-parameter PeekTC<CTCShapeManager> (interface 0x44; the uninitialised out pointer read from
//   its home slot on the absent path, `mov eax,[esp+20h]`); the track and the length fraction are uninitialised
//   locals filled through out pointers; the nearest point is a named local returned through C3DVector's memberwise
//   copy constructor. DragCamPos +0x48 is passed by reference (`add edi,48h`).
// Layout: Ego_r.pdb CTCCameraPointTrack: CameraStringLength +0x40, CameraCageRadius +0x44, DragCamPos +0x48 (as the
//   landed UpdateActiveCameraPosition 0x0075CAD0); engine/CTCBase.h (ParentThing +4).
#include "engine/CTCBase.h"

#include "engine/C3DVector.h"

namespace NShape {
class CTrack {
public:
    void GetTrackPosNearest(const C3DVector& pos, C3DVector& nearest, float& fraction) const;   // 0x00760DA0
};
}

class CTCShapeManager {
public:
    bool GetBestTrack(const NShape::CTrack** out) const;   // 0x00761B50
};

class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCameraPointTrack : public CTCBase {
public:
    unsigned char _pad0c[0x40 - 0x0c];
    float CameraStringLength;                           // +0x40
    float CameraCageRadius;                             // +0x44
    C3DVector DragCamPos;                               // +0x48
    C3DVector GetActiveCameraPosition() const;         // vtable slot 35 (CTCBase.h models the vptr as a member)
};

C3DVector CTCCameraPointTrack::GetActiveCameraPosition() const
{
    const const CTCShapeManager* shapes;
    ParentThing->PeekTC(TCI_SHAPE_MANAGER, &shapes);
    const NShape::CTrack* track;
    shapes->GetBestTrack(&track);
    C3DVector nearest;
    float fraction;
    track->GetTrackPosNearest(DragCamPos, nearest, fraction);
    return nearest;
}