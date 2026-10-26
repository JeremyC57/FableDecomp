// CTCCreatureNavigation::GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result) -- ask the navigator for a flee position from the centre; true when it is on the world map and not at the centre
// Grade: RELOCATION_MATCH -- the squared distance is accumulated in statements (`r = dx*dx; r += dz*dz; r += dy*dy;`), which fixes the x87 square order to dx, dz, dy as retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB840, length 153 bytes (+ int3 padding).
// Callees: CNavigatorBase slot 7 (+0x1C, flee = true), CTCBase::GetWorldMap @ 0x00686D20, CWorldMap::IsPosInMap @ 0x004FC600.
// Shape: nested ifs with `return true; ... return false;` give the byte exits.
// Layout: Ego_r.pdb CTCCreatureNavigation: PNavigator +0xC (the active CNavigatorBase). Retail CThing::GetPosition @ 0x004C73D0.
// Globals: the 1e-4f tolerance @ 0x0129BA3C is an extern float variable (`fld [g] ; fmul [g]` squares it).
class C3DVector { public:
    float X, Y, Z;
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
};
inline float DistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    float r = dx * dx;
    r += dz * dz;
    r += dy * dy;
    return r;
}
inline float XYDistanceSquared(const C3DVector& a, const C3DVector& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    return dx * dx + dy * dy;
}
extern float g_fTolerance;                      // 0x0129BA3C (1e-4f)
class CThing { public:
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CWorldMap { public:
    bool IsPosInMap(const C3DVector& pos) const;      // 0x004FC600
};
class CNavigatorBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual C3DVector GetRandomPosition(const C3DVector& centre, float radius, bool flee);     // slot 7 (+0x1c)
    virtual void v08();
    virtual bool IsAbleToNavigate(const C3DVector& from, const C3DVector& to, float proximity, bool fromCurrentPos);   // slot 9 (+0x24)
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CWorldMap& PeekWorldMap() const;   // 0x00686D20
};

class CTCCreatureNavigation : public CTCBase { public:
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CNavigatorBase* PNavigator;                 // +0x0c
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity);
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool flag);
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);
};

bool CTCCreatureNavigation::GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result)
{
    *result = PNavigator->GetRandomPosition(centre, radius, true);
    if (PeekWorldMap().IsPosInMap(*result)) {
        if (DistanceSquared(*result, centre) > g_fTolerance * g_fTolerance)
            return true;
    }
    return false;
}