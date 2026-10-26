// CTCCreatureNavigation::GetNearbyDirectedPosition(const C3DVector& from, float, const C3DVector& direction, float, C3DVector* out) -- ask the navigator for a nearby position along the direction; true when it is on the map and not (within epsilon) the start point
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB780, 180 bytes (`ret 14h`).
// Callees: the navigator's virtual slot 6 (+0x18; returns a C3DVector by hidden pointer, takes the direction's XY as a
// const C2DVector&), CTCBase::PeekWorldMap() const @ 0x00686D20, CWorldMap::IsPosInMap(const C3DVector&) @ 0x004FC600;
// g_epsilon_0129ba3c @ 0x0129BA3C (1e-4f).
// Layout: Ego_r.pdb CTCCreatureNavigation PNavigator +0x0c (before any container).
// Shape: the XY copy of the direction is a C2DVector temporary in the frame; the navigator's result temporary is copied
// into *out; the difference is an inline operator- whose named local keeps all three components on the x87 stack, and the
// length squared is summed z first (`s = Z*Z; s += X*X; s += Y*Y`, GOTCHAS retry 8), compared against eps*eps.
extern float g_epsilon_0129ba3c;                        // 0x0129BA3C (1e-4f)
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector d;
        d.X = X - o.X;
        d.Y = Y - o.Y;
        d.Z = Z - o.Z;
        return d;
    }
    float GetLengthSquared() const
    {
        float s = Z * Z;
        s += X * X;
        s += Y * Y;
        return s;
    }
};
class CNavigatorBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual C3DVector GetNearbyDirectedPosition(const C3DVector& from, float distance, const C2DVector& direction, float spread);   // slot 6 (+0x18)
};
class CWorldMap { public:
    bool IsPosInMap(const C3DVector& position) const;   // 0x004FC600
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CWorldMap& PeekWorldMap() const;   // 0x00686D20
};

class CTCCreatureNavigation : public CTCBase { public:
    unsigned char _pad04[0x0c - 0x04];
    CNavigatorBase* PNavigator;                         // +0x0c
    bool GetNearbyDirectedPosition(const C3DVector& from, float distance, const C3DVector& direction, float spread, C3DVector* out);
};

bool CTCCreatureNavigation::GetNearbyDirectedPosition(const C3DVector& from, float distance, const C3DVector& direction, float spread, C3DVector* out)
{
    *out = PNavigator->GetNearbyDirectedPosition(from, distance, C2DVector(direction.X, direction.Y), spread);
    if (PeekWorldMap().IsPosInMap(*out)) {
        C3DVector delta = *out - from;
        if (delta.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            return true;
    }
    return false;
}