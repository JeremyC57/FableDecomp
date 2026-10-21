// NShape::CTrack::CTrack(CTCShapeManager* owner, const vector<C3DVector>& points) -- shape base (owner, copied point list, empty selection), zero traverser and length, then sum the segment lengths into TrackLength
// Copy constructor: the C3DVector model DECLARES its memberwise copy constructor (retail's header class has one); with the
// implicit copy VC7.1 schedules the copies differently (found by the retry-10 copy-constructor sweep).
// Leaf: ??0CTrack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00761F40, length 208 bytes (`ret 8`).
// Callees: vector<C3DVector>::vector(const vector&) @ 0x006F0810 (manifest "Vector_CopyFrom").
// Identity: vtables 0x0126A208 (NShape::CShape) then 0x0126A288 (NShape::CTrack) per rtti-vtables.tsv.
// Layout (Ego_r.pdb CShape/CTrack with 12-byte retail vectors): Owner +0x04, Points +0x08, SelectedPoints +0x14,
// TrackTraverser +0x20, TrackLength +0x24.
// Shape: the CShape constructor is inline (its vtable store first, the owner, the out-of-line point-list copy, the empty
// selection vector); the derived zero initialisers share the selection's zero register before the CTrack vtable store;
// the body walks the points with post-incremented iterator reads (`*it++`, so the step sits at the loop top) and the
// previous point copied into a local, and accumulates `TrackLength += |p - prev|`
// (`fsqrt` intrinsic; the squares are written x, y, z so VC7.1's right-first FPU order loads z first).
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float x, y, z;
    C3DVector() { } C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
struct CTCShapeManager;
struct CPointVector {                                       // STLport vector<C3DVector>, 12 bytes in retail
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CPointVector(const CPointVector& other);                // 0x006F0810
    C3DVector* begin() const { return _M_start; }
    C3DVector* end() const { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
struct CSelectionVector {                                   // vector<vector<C3DVector>::iterator>, 12 bytes in retail
    C3DVector** _M_start;
    C3DVector** _M_finish;
    C3DVector** _M_end_of_storage;
    CSelectionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

namespace NShape {
struct CShape {                                             // vtable 0x0126A208
    virtual void v00();
    CTCShapeManager* Owner;                                 // +0x04
    CPointVector     Points;                                // +0x08
    CSelectionVector SelectedPoints;                        // +0x14
    CShape(CTCShapeManager* owner, const CPointVector& points) : Owner(owner), Points(points) { }
};
struct CTrack : CShape {                                    // vtable 0x0126A288
    long  TrackTraverser;                                   // +0x20
    float TrackLength;                                      // +0x24
    CTrack(CTCShapeManager* owner, const CPointVector& points);
    virtual void v00();
};
}

NShape::CTrack::CTrack(CTCShapeManager* owner, const CPointVector& points)
    : CShape(owner, points), TrackTraverser(0), TrackLength(0)
{
    if (Points.size() > 1) {
        C3DVector* it = Points.begin();
        C3DVector previous = *it;
        ++it;
        while (it != Points.end()) {
            const C3DVector& point = *it++;
            float dx = point.x - previous.x;
            float dy = point.y - previous.y;
            float dz = point.z - previous.z;
            TrackLength += (float)sqrt(dx * dx + dy * dy + dz * dz);
            previous = point;
        }
    }
}