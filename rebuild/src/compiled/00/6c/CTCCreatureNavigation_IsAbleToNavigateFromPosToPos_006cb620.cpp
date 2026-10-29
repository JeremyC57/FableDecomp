// CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) -- ask the active navigator whether the route is possible, telling it whether the start is the creature's own position
// Grade: RELOCATION_MATCH -- the squared distance is accumulated in statements (`r = dz*dz; r += dx*dx; r += dy*dy;`), which fixes the x87 square order to dz, dx, dy as retail; a single-expression sum reorders it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB620, length 114 bytes (+ int3 padding).
// Callees: CThing::GetPos @ 0x004C73D0, CNavigatorBase slot 9 (+0x24).
// Shape: the bool local is set true first and cleared when the start is farther than the tolerance from the parent's position
// (`fcompp ; test ah,5 ; jp`); the difference vector stays on the x87 stack.
// Layout: Ego_r.pdb CTCCreatureNavigation: PNavigator +0xC (the active CNavigatorBase). Retail CThing::GetPos @ 0x004C73D0.
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
    float r = dz * dz;
    r += dx * dx;
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
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CWorldMap { public:
    bool IsPosInMap(const C3DVector& pos);      // 0x004FC600
};
class CNavigatorBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual C3DVector GetRandomPosition(const C3DVector& centre, float radius, bool flee);     // slot 7 (+0x1c)
    virtual void v08();
    virtual bool IsAbleToNavigate(const C3DVector& from, const C3DVector& to, float proximity, bool fromCurrentPos);   // slot 9 (+0x24)
};
class CTCCreatureNavigation { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CNavigatorBase* PNavigator;                 // +0x0c
    CWorldMap* GetWorldMap() const;             // 0x00686D20 (CTCBase; reads a global, the manifest's "GetDisplayEngine" is propagated)
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const;
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool flag);
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);
};

bool CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float proximity) const
{
    bool fromCurrentPos = true;
    if (DistanceSquared(from, ParentThing->GetPos()) > g_fTolerance * g_fTolerance)
        fromCurrentPos = false;
    return PNavigator->IsAbleToNavigate(from, to, proximity, fromCurrentPos);
}