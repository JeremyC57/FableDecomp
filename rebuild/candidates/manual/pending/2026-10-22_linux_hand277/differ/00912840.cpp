// CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath(std::deque<CBezierCurve>&) -- continue the path smoothly from its last curve: mirror the previous control point (halving it until it is above ground, on the map and in the same map region), then pick two random flight positions for the remaining control points and append the curve
// Grade: DIFFER(860v860) -- only the push_back curve copy: retail copies the 12 dwords software-pipelined (next load before the previous store), ours pairs each load/store per C3DVector copy ctor; HeightProbeOffset.Y/.Z show as symbol+4/+8 (as in landed 0x912700)
#include <new>
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
struct CBezierCurve {                              // Layout: Ego_r.pdb CBezierCurve, 4 points, 0x30
    C3DVector Point1;
    C3DVector Point2;
    C3DVector Point3;
    C3DVector Point4;
};
struct CBezierIterator {                           // STLport deque iterator, 2 curves per 0x60 buffer
    CBezierCurve* _M_cur;
    CBezierCurve* _M_first;
    CBezierCurve* _M_last;
    CBezierCurve** _M_node;
    void _M_set_node(CBezierCurve** node) { _M_node = node; _M_first = *node; _M_last = _M_first + 2; }
    CBezierIterator& operator--()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
        return *this;
    }
    CBezierCurve& operator*() const { return *_M_cur; }
};
struct CBezierDeque {
    CBezierIterator _M_start;
    CBezierIterator _M_finish;                     // +0x10
    CBezierCurve** _M_map;
    unsigned long _M_map_size;
    CBezierCurve& back() { CBezierIterator tmp = _M_finish; --tmp; return *tmp; }
    void _M_push_back_aux_v(const CBezierCurve& x);   // 0x00913D60
    void push_back(const CBezierCurve& x)
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1) {
            new (_M_finish._M_cur) CBezierCurve(x);
            ++_M_finish._M_cur;
        } else
            _M_push_back_aux_v(x);
    }
};
struct CRandomSeed;
float __fastcall GFFloatRandom(CRandomSeed* seed, float range);   // 0x004978A0
extern const C3DVector HeightProbeOffset;          // 0x0138C500
struct CThingFlying {
    float GetHeightZBelowAt(const C3DVector& pos, bool a, bool b);   // 0x00666D40
};
struct CWorldMapMethods {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;           // slot 16
};
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {   // Layout: Ego_r.pdb, base 0x14
    float MinHeightToKeepAboveGround;              // +0x14
    float MinHeightToKeepAboveGroundInFlight;      // +0x18
    float MaxHeightAboveGround;                    // +0x1c
    float FlightRange;                             // +0x20
    CThingFlying* GetCreature() const { return (CThingFlying*)PBrain->Creature; }
    CWorldMapMethods* PeekWorldMap();              // 0x008FCFF0
    CRandomSeed* GetPWorldSeed();                  // 0x008FCFD0
    bool GetRandomFlightPos(const C3DVector& from, float radius, C3DVector* out, bool allowGround);   // 0x00912700
    __forceinline float RandomFloat(const float& range) { return GFFloatRandom(GetPWorldSeed(), range); }
    bool AddRandomBezierCurveToPath(CBezierDeque& path);
};

bool CAIStateGroup_FlyingThingBase::AddRandomBezierCurveToPath(CBezierDeque& path)
{
    CBezierCurve curve;
    C3DVector end = path.back().Point4;
    curve.Point1 = end;
    C3DVector dir = end - path.back().Point3;
    C3DVector control = dir + end;
    long mapNumber = PeekWorldMap()->GetMapNumberAt(end);
    float height = GetCreature()->GetHeightZBelowAt(HeightProbeOffset + control, true, true);
    long tries = 10;
    while ((height + MinHeightToKeepAboveGroundInFlight > control.Z || control.X < 0.0f || control.Y < 0.0f
            || PeekWorldMap()->GetMapNumberAt(control) != mapNumber) && tries > 0) {
        dir *= 0.5f;
        control = dir + end;
        height = GetCreature()->GetHeightZBelowAt(HeightProbeOffset + control, true, true);
        --tries;
    }
    curve.Point2 = control;
    for (unsigned long i = 0; i < 2; ++i) {
        C3DVector pos;
        if (!GetRandomFlightPos(end, FlightRange, &pos, false))
            return false;
        switch (i) {
        case 0:
            pos.Z = pos.Z + MinHeightToKeepAboveGroundInFlight;
            curve.Point3 = pos;
            break;
        case 1: {
            CRandomSeed* seed = GetPWorldSeed();
            pos.Z = GFFloatRandom(seed, MaxHeightAboveGround) + pos.Z + MinHeightToKeepAboveGroundInFlight;
            curve.Point4 = pos;
            break;
        }
        }
    }
    path.push_back(curve);
    return true;
}
