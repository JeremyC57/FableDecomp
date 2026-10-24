// CTCCameraPointTrack::GetActiveCameraPosition() const -- the camera position: the point of the parent's best track nearest the drag-camera position
// Leaf: ?GetActiveCameraPosition@
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

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

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

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> bool PeekTC(long type, const T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};

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
    const CTCShapeManager* shapes;
    ParentThing->PeekTC(0x44, &shapes);
    const NShape::CTrack* track;
    shapes->GetBestTrack(&track);
    C3DVector nearest;
    float fraction;
    track->GetTrackPosNearest(DragCamPos, nearest, fraction);
    return nearest;
}