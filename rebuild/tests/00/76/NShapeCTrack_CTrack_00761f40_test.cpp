// Behaviour test for 00761f40: src/00761f40.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NShape::CTrack::CTrack(CTCShapeManager* owner, const vector<C3DVector>& points)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NShape::CTrack::CTrack(CTCShapeManager* owner, const vector<C3DVector>& points) -- shape base (owner, copied point list, empty selection), zero traverser and length, then sum the segment lengths into TrackLength
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
class C3DVector { public:
    float x, y, z;
    C3DVector() { } C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
class CTCShapeManager;
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
class CShape { public:                                             // vtable 0x0126A208
    virtual void v00();
    CTCShapeManager* Owner;                                 // +0x04
    CPointVector     Points;                                // +0x08
    CSelectionVector SelectedPoints;                        // +0x14
    CShape(CTCShapeManager* owner, const CPointVector& points) : Owner(owner), Points(points) { }
};
class CTrack : public CShape { public:                                    // vtable 0x0126A288
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

// ---- stubs and driver ----
void NShape::CShape::v00() {}
void NShape::CTrack::v00() {}
#include <stdlib.h>
static int g_copies = 0;
CPointVector::CPointVector(const CPointVector& other)
{
    ++g_copies;
    unsigned long n = other.size();
    _M_start = (C3DVector*)malloc(n * sizeof(C3DVector) + 1);
    for (unsigned long i = 0; i < n; ++i) _M_start[i] = other._M_start[i];
    _M_finish = _M_end_of_storage = _M_start + n;
}
static C3DVector V(float x, float y, float z) { C3DVector v; v.x = x; v.y = y; v.z = z; return v; }
int main()
{
    C3DVector pts[3] = { V(0, 0, 0), V(3, 4, 0), V(3, 4, 12) };
    static unsigned char srcStore[sizeof(CPointVector)]; CPointVector& src = *(CPointVector*)srcStore; src._M_start = pts; src._M_finish = pts + 3; src._M_end_of_storage = pts + 3;
    CTCShapeManager* owner = (CTCShapeManager*)0x4444;
    NShape::CTrack track(owner, src);
    CHECK(g_copies == 1); CHECK(track.Owner == owner); CHECK(track.Points.size() == 3); CHECK(track.Points._M_start != pts);
    CHECK(track.TrackTraverser == 0); CHECK(track.TrackLength == 17.0f);
    CHECK(track.SelectedPoints._M_start == 0 && track.SelectedPoints._M_finish == 0 && track.SelectedPoints._M_end_of_storage == 0);
    src._M_finish = pts + 1;
    NShape::CTrack single(owner, src);
    CHECK(single.TrackLength == 0.0f); CHECK(g_copies == 2);
    src._M_finish = pts + 2;
    NShape::CTrack two(owner, src);
    CHECK(two.TrackLength == 5.0f);
    if (fails == 0) printf("00761f40_TEST PASS\n"); else printf("00761f40_TEST FAIL\n");
    return fails;
}