// Behaviour test for 0x0075DC60 CTCCameraPointTrack::GetActiveCameraPosition: finds the parent's interface 0x44 shape
// manager, takes its best track and returns the track point nearest the drag-camera position (DragCamPos is the query).
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace NShape {
class CTrack {                                   // a straight track from A to B sampled at 0.1 steps
public:
    C3DVector A, B;
    void GetTrackPosNearest(const C3DVector& pos, C3DVector& nearest, float& fraction) const
    {
        float best = 1e10f;
        for (int i = 0; i <= 10; ++i) {
            float t = i * 0.1f;
            C3DVector p(A.X + (B.X - A.X) * t, A.Y + (B.Y - A.Y) * t, A.Z + (B.Z - A.Z) * t);
            float dx = p.X - pos.X, dy = p.Y - pos.Y, dz = p.Z - pos.Z;
            float d = dx * dx + dy * dy + dz * dz;
            if (d < best) { best = d; nearest = p; fraction = t; }
        }
    }
};
}

class CTCShapeManager {
public:
    const NShape::CTrack* m_track;
    bool GetBestTrack(const NShape::CTrack** out) const
    {
        if (!m_track) return false;
        *out = m_track;
        return true;
    }
};

struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool PeekTC(long type, const T** out) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap::SEntry* it = m_interfaces.LowerBound(&key);
            if (it == m_interfaces.last || type < it->type)
                it = m_interfaces.last;
            *out = (const T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCCameraPointTrack { public:
    CThing* ParentThing;
    float CameraStringLength;
    float CameraCageRadius;
    C3DVector DragCamPos;
    C3DVector GetActiveCameraPosition() const
    {
        const CTCShapeManager* shapes = 0;
        ParentThing->PeekTC(0x44, &shapes);
        const NShape::CTrack* track = 0;
        shapes->GetBestTrack(&track);
        C3DVector nearest;
        float fraction;
        track->GetTrackPosNearest(DragCamPos, nearest, fraction);
        return nearest;
    }
};

int main()
{
    NShape::CTrack track; track.A = C3DVector(0.0f, 0.0f, 0.0f); track.B = C3DVector(10.0f, 0.0f, 0.0f);
    CTCShapeManager shapes; shapes.m_track = &track;
    CInterfaceMap::SEntry entries[2] = { { 0x3f, 0 }, { 0x44, &shapes } };
    CThing thing;
    for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_presentBits[2] = 0x10UL;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
    CTCCameraPointTrack cam; cam.ParentThing = &thing;
    cam.CameraStringLength = 5.0f; cam.CameraCageRadius = 1.0f;
    cam.DragCamPos = C3DVector(6.9f, 3.0f, 0.0f);            // nearest sample is t = 0.7 -> (7, 0, 0)
    C3DVector r = cam.GetActiveCameraPosition();
    int ok = 1;
    if (r.X < 6.99f || r.X > 7.01f || r.Y != 0.0f || r.Z != 0.0f) ok = 0;
    cam.DragCamPos = C3DVector(-4.0f, 0.0f, 0.0f);           // before the start: clamps to A
    r = cam.GetActiveCameraPosition();
    if (r.X != 0.0f) ok = 0;
    if (!ok) { std::printf("0075dc60_TEST FAIL\n"); return 1; }
    std::printf("0075dc60_TEST PASS\n");
    return 0;
}